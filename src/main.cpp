#include "CSGOMatchList.h"
#include "BoilerException.h"

#include <steam/steam_api.h>

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <thread>


void Error(const std::string &title, const std::string &text) {
    std::cerr << title << ": " << text << std::endl;
}

bool file_exists(const std::string &name) {
    std::ifstream f(name);
    bool ret = f.good();
    f.close();
    return ret;
}

CMsgGCCStrike15_v2_MatchmakingServerRoundStats* getLegacyRoundStats(CDataGCCStrike15_v2_MatchInfo& match)
{
    if (match.has_roundstats_legacy())
        return match.mutable_roundstats_legacy();
    for (auto i = 0; i < match.roundstatsall_size(); ++i)
        if (match.roundstatsall(i).has_map() && match.roundstatsall(i).has_reservationid())
            return match.mutable_roundstatsall(i);
    return nullptr;
}

std::string dotDem(const CDataGCCStrike15_v2_MatchInfo& match, const CMsgGCCStrike15_v2_MatchmakingServerRoundStats& rs) {
    std::ostringstream out;
    out << "match730_" << std::setfill('0') << std::setw(21) << rs.reservationid()
        << "_" << std::setw(10) << match.watchablematchinfo().tv_port() << "_"
        << match.watchablematchinfo().server_ip() << ".dem";
    return out.str();
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: ./broiler dir_with_replays" << std::endl;
        return 1;
    }
    std::string directory(argv[1]);

    // init steam
    if (SteamAPI_RestartAppIfNecessary(k_uAppIdInvalid))
        return 1;
    if (!SteamAPI_Init()) {
        Error("Fatal Error", "Steam must be running (SteamAPI_Init() failed).\n");
        return 1;
    }
    if (!SteamUser()->BLoggedOn()) {
        Error("Fatal Error",
              "Steam user must be logged in (SteamUser()->BLoggedOn() returned false).\n");
        return 1;
    }

    bool running = true;
    auto cbthread = std::thread([&running]() {
        while (running) {
            try {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
                Steam_RunCallbacks(GetHSteamPipe(), false);
            } catch (BoilerException &e) {
                Error("Fatal Error", e.what());
                exit(1);
            }
        };
    });

    int res = 0;

    try {
        // make sure we are connected to the gc
        CSGOClient::GetInstance()->WaitForGcConnect();

        // refresh match list
        CSGOMatchList refresher;
        refresher.RefreshWait();
        std::vector<CDataGCCStrike15_v2_MatchInfo> matches = refresher.Matches();
        for (auto &match : matches) {
            CMsgGCCStrike15_v2_MatchmakingServerRoundStats* rs = getLegacyRoundStats(match);
            if (!rs) {
                std::cerr << "Cannot get info for match" << std::endl;
                continue;
            }
            std::string dotDemFile = directory + "/" + dotDem(match, *rs);
            std::string dotDemDotInfoFile = dotDemFile + ".info";
            std::string link = rs->map();

            if (!file_exists(dotDemDotInfoFile)) {
                rs->clear_map();
                std::cerr << "Creating " << dotDemDotInfoFile << std::endl;
                std::ofstream myfile;
                myfile.open(dotDemDotInfoFile, std::ios::binary);
                if (!match.SerializeToOstream(&myfile))
                    Error("Error", std::string("Cannot serialize to file ") + dotDemDotInfoFile);
                myfile.close();
            }

            if (!file_exists(dotDemFile))
                std::cout << link << std::endl;
        }
    } catch (BoilerException &e) {
        Error("Fatal error", e.what());
        res = 1;
    }

    // shutdown
    running = false;
    cbthread.join();
    CSGOClient::Destroy();

    SteamAPI_Shutdown();
    return res;
}