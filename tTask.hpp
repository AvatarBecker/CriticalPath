#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#ifndef __TTASK_HPP__
#define __TTASK_HPP__

class tTask
{
    public:
        static std::vector<tTask*> task_list;

        int                 id;
        std::string         name;
        struct tm           duration;
        std::vector<tTask*> depcies; // will be initialized with the pointers to the tasks it depends on

        tTask();
        tTask(int id, std::string name, struct tm duration);
        int  AddDependency(tTask* depcy);
        void Print(std::ostream& medium);
        void PrintShort(std::ostream& medium);
};

int  ParseForTaskData(std::string& line);//, vector<tTask*>& task_list);

struct tm    TmSum(struct tm& time_1, struct tm& time_2);
unsigned int TmToUnsigned(struct tm& time_tm);
struct tm    UnsignedToTm(unsigned int time_uns);
void         PrintTm(struct tm time_tm, std::ostream& medium);

class tPath
{
    public:
        static std::vector<tPath*> path_list;

        unsigned int        total_time;  // don't alter by hand
        std::vector<tTask*> tasks;       // ditto

        void AddTask(tTask* task);

        tPath();
        tPath(bool dont_push);
        tPath(tPath& lhs);

        void Print(std::ostream& medium);
};

int         FindPathToStart(tTask* curr_task, tPath* curr_path);
inline bool ShorterThen(tPath* lhs, tPath* rhs)
            {return lhs->total_time < rhs->total_time; }

extern tTask cStart;

#endif // __TTASK_HPP__
