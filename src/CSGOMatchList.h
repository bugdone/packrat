#pragma once
#include <condition_variable>
#include "CSGOClient.h"


/**
 * utility class to store and update the match list
 */
class CSGOMatchList
{
public:
    CSGOMatchList();
    ~CSGOMatchList();

    void Refresh();
    void RefreshWait();

    const std::vector<CDataGCCStrike15_v2_MatchInfo>& Matches() const;

private:
    void OnClientWelcome(const CMsgClientWelcome& msg);
    void OnMatchList(const CMsgGCCStrike15_v2_MatchList& msg);

private:
    bool m_updateComplete;
    std::condition_variable m_updateCv;
    mutable std::mutex m_matchMutex;
    std::vector<CDataGCCStrike15_v2_MatchInfo> m_matches;
    GCMsgHandler<CMsgGCCStrike15_v2_MatchList> m_matchListHandler;
};
