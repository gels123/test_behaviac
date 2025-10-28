#include <stdio.h>
#include <string>
#include <cstdio>
#include <list>

#include "ai_mgr.h"
#include "cpp/behaviac_generated/types/behaviac_types.h"

#define LOGI printf

#if BEHAVIAC_CCDEFINE_MSVC
#include <tchar.h>
#include <windows.h>
#elif defined(__linux__) || defined(__unix__)
#include <unistd.h>
#include <limits.h>
#endif

AiEntityMgr::AiEntityMgr() 
{
}

AiEntityMgr::~AiEntityMgr() 
{
  if (b_init) {
    b_init = false;
    behaviac::Workspace::GetInstance()->Cleanup();
    behaviac::DestroyBehaviorLoader();
  }
}

void AiEntityMgr::InitExePath() 
{
#if defined(_WIN32) || defined(_WIN64) || BEHAVIAC_CCDEFINE_MSVC
  TCHAR szCurPath[_MAX_PATH];

  GetModuleFileName(NULL, szCurPath, _MAX_PATH);
  TCHAR* p = szCurPath;
  // 修复C28183警告：检查空指针
  while (p && _tcschr(p, L'\\')) {
    p = _tcschr(p, L'\\');
    p++;
  }
  *p = L'\0';
  SetCurrentDirectory(szCurPath);
  // 检查打印一下
  TCHAR curDir[_MAX_PATH];
  DWORD ret = GetCurrentDirectory(MAX_PATH, curDir);
  if (ret > 0) {
    LOGI("curDir=%ls\n", curDir); 
  }
#elif defined(__linux__) || defined(__unix__)

#elif defined(__APPLE__) && defined(__MACH__)

#endif
}

bool AiEntityMgr::InitBehavic(std::string filepath) {
  //LOGI("InitBehavic\n");
  if (b_init) {
    return true;
  }
  b_init = true;
  auto workspace = behaviac::Workspace::GetInstance();
  workspace->SetFilePath(filepath.c_str());
  workspace->SetFileFormat(behaviac::Workspace::EFF_xml);

  behaviac::Config::SetLogging(true);
  behaviac::Config::SetSocketBlocking(true);

  behaviac::InitBehaviorLoader();

  return true;
}

bool AiEntityMgr::CreateAgent(AgentId agentId, std::string btName) {
  //LOGI("InitAgent\n");
  if (m_agents.find(agentId) != m_agents.end()) {
    LOGI("Agent with id %u already exists.\n", agentId);
    return true;
  }
  FirstAgent* agent = behaviac::Agent::Create<FirstAgent>();
  bool ret = agent->btload(btName.c_str());
  if (!ret) {
    LOGI("Failed to load behavior tree: %s\n", btName.c_str());
    return false;
  }
  agent->SetIdFlag(agentId);
  agent->btsetcurrent(btName.c_str());
  m_agents.insert({agentId, std::make_shared<AiAgent>(agentId, agent, behaviac::EBTStatus::BT_RUNNING)});
  return true;
}

bool AiEntityMgr::DeleteAgent(AgentId agentId) {
  auto it = m_agents.find(agentId);
  if (it == m_agents.end()) return false;
  m_agents.erase(it);
  return true;
}

bool AiEntityMgr::UpdateLoop() {
  //LOGI("UpdateLoop\n");
  std::list<AgentId> lrm; 
  for (auto it = m_agents.begin(); it != m_agents.end();) {
    AiAgentPtr ai_agent = it->second; 
    ++(ai_agent->frame);
    ai_agent->status = ai_agent->agent->btexec();
    if (ai_agent->status != behaviac::EBTStatus::BT_RUNNING) {
      it = m_agents.erase(it);
    } else {
      ++it;
    }
  }
  return !m_agents.empty();
}


