#pragma once
#include <stdio.h>
#include <string>
#include <map>
#include "cpp/behaviac_generated/types/behaviac_types.h"

#define AI_API __declspec(dllexport)

typedef uint32_t AgentId;

class AiAgent {
 public:
  AgentId id = 0;
  behaviac::Agent* agent = nullptr;
  behaviac::EBTStatus status = behaviac::EBTStatus::BT_INVALID;
  uint64_t frame = 0;

  AiAgent(AgentId agentId, behaviac::Agent* agent, behaviac::EBTStatus status): id(agentId),agent(agent),status(status) {}
  ~AiAgent() {
    if (agent != nullptr) {
      behaviac::Agent::Destroy(agent);
      agent = nullptr;
    }
  }
};

typedef std::shared_ptr<AiAgent> AiAgentPtr;

class /*AI_API*/ AiEntityMgr {
  friend class AiAgent;

 public:
  AiEntityMgr();
  virtual ~AiEntityMgr();

  void InitExePath();
  bool InitBehavic(std::string filePath);
  bool CreateAgent(AgentId agentId, std::string btName);
  bool DeleteAgent(AgentId agentId);
  bool UpdateLoop();

private:
  bool b_init = false;
  std::map<AgentId, AiAgentPtr> m_agents{};
};
