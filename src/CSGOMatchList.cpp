#include "CSGOMatchList.h"
#include <iostream>
#include "BoilerException.h"

CSGOMatchList::CSGOMatchList()
    :m_matchListHandler(this, &CSGOMatchList::OnMatchList)
{
    CSGOClient::GetInstance()->RegisterHandler(k_EMsgGCCStrike15_v2_MatchList, &m_matchListHandler);
}

CSGOMatchList::~CSGOMatchList()
{
    CSGOClient::GetInstance()->RemoveHandler(k_EMsgGCCStrike15_v2_MatchList, &m_matchListHandler);
}

void CSGOMatchList::OnMatchList(const CMsgGCCStrike15_v2_MatchList& msg)
{
    std::unique_lock<std::mutex> lock(m_matchMutex);
    for (auto it = msg.matches().rbegin(); it != msg.matches().rend(); ++it)
        m_matches.push_back(*it);
    m_updateComplete = true;
    lock.unlock();
    m_updateCv.notify_all();
}

void CSGOMatchList::Refresh()
{
    CMsgGCCStrike15_v2_MatchListRequestRecentUserGames request;
    request.set_accountid(SteamUser()->GetSteamID().GetAccountID());
    if (CSGOClient::GetInstance()->SendGCMessage(k_EMsgGCCStrike15_v2_MatchListRequestRecentUserGames, &request) != k_EGCResultOK)
        throw BoilerException("Failed to send EMsgGCCStrike15_v2_MatchListRequestRecentUserGames");
}

void CSGOMatchList::RefreshWait()
{
    m_updateComplete = false;
    Refresh();
    std::unique_lock<std::mutex> lock(m_matchMutex);
    while (!m_updateComplete)
        m_updateCv.wait(lock);
}

const std::vector<CDataGCCStrike15_v2_MatchInfo>& CSGOMatchList::Matches() const
{
    std::lock_guard<std::mutex> lock(m_matchMutex);
    return m_matches;
}
