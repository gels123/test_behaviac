// ConsoleApplication1.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include "../ai/ai_mgr.h"

int main()
{
    std::cout << "Hello World!\n";
    {
      AiEntityMgr mgr;

      mgr.InitExePath();

      std::string path = "../../ai/cpp/exported";
      mgr.InitBehavic(path);

      mgr.CreateAgent(101, "FirstBT");
      //mgr.CreateAgent(102, "FirstBT");

      while (mgr.UpdateLoop());
      mgr.DeleteAgent(101);
      mgr.DeleteAgent(102);
    }
    return 0;
}


