#include "tTask.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

vector<tTask*> tTask::task_list;
vector<tPath*> tPath::path_list;

tTask::tTask()
{
    id = 0;
    name = "nome";
    duration = {0};
}

tTask::tTask(int id_temp, string name_temp, struct tm duration_temp)
{
    id = id_temp;
    name = name_temp;
    duration = duration_temp;

    task_list.push_back(this);
}

struct tm dur_tm = {0};
tTask cStart(0, "Inicio", dur_tm);

int tTask::AddDependency(tTask* depcy)
{
    depcies.push_back(depcy);

    return 0;
}

void tTask::Print(ostream& medium)
{
    medium << "ID: " << id << endl;
    medium << "Nome: " << name << endl;
    medium << "Duracao: ";
    PrintTm(duration, medium);
    cout << endl;
    medium << "Dependencias: ";

    for(size_t i=0; i<depcies.size(); i++)
    {
        medium << depcies[i]->id << ", ";
    }

    medium << endl;
}

void tTask::PrintShort(ostream& medium)
{
    medium << "Entrada." << id << " - ";
    medium << name << " - ";
    PrintTm(duration, medium);

    medium << endl;
}

int ParseForTaskData(string& line)//, vector<tTask*>& task_list)
{
    // Create task and add to task_list vector:
    tTask* task = (tTask*)malloc(sizeof(tTask));

    size_t pos_i;
    size_t pos_f;
    int    task_id;
    string task_name;
    struct tm task_duration = {0};

    // Task ID:
    pos_i   = line.find_first_of('.');

    if(pos_i==string::npos)
    {
        return -1;
    }

    pos_i   = line.find_first_not_of(' ', pos_i) + 1;
    pos_f   = line.find_first_not_of("1234567890",pos_i);

    string task_id_str;

    for( size_t pos = pos_i; pos < pos_f; ++pos )
    {
        task_id_str.push_back(line[pos]);
    }
    task_id = atoi(task_id_str.c_str());

    // Task Name:
    pos_i = pos_f;
    pos_i = line.find_first_not_of(" -", pos_i);
    pos_f = line.find_first_of("(", pos_i) - 1;

    if(pos_f == string::npos)
    {
        return -1;
    }

    task_name.assign(line, pos_i, pos_f - pos_i + 1);

    // Task Duration:
    size_t pos = pos_f + 2;

    while(line[pos] != ')')
    {
        string number_str;
        char   time_unit;

        pos_f = line.find_first_not_of("1234567890",pos);

        while(pos < pos_f)
        {
            number_str.push_back(line[pos]);
            ++pos;
        }

        time_unit = line[pos];

        switch(time_unit)
        {
            case 'a':
                task_duration.tm_year = atoi(number_str.c_str());
                break;
            case 'm':
                task_duration.tm_mon  = atoi(number_str.c_str());
                break;
            case 'd':
                task_duration.tm_yday = atoi(number_str.c_str());
                break;
            case 'h':
                task_duration.tm_hour = atoi(number_str.c_str());
                break;
            default:

                cout << endl << "#ERRO: Unidade de tempo faltante ou incorreta." << endl << endl;
                system("PAUSE");

                return -1;
                break;
        }

        pos++;

    }

    // Create task object in previously allocated memory:
    new (task) tTask(task_id, task_name, task_duration);

    // Task Dependencies:
    size_t pos_D = line.find_first_of('D');
    pos = pos_D + 1;

    if(pos_D != string::npos)
    {
        while(line[pos] != ')')
        {
            string  number_str;
            int     number;

            pos_f = line.find_first_not_of("1234567890",pos);

            while(pos < pos_f)
            {
                number_str.push_back(line[pos]);
                ++pos;
            }

            number = atoi(number_str.c_str());

            if(number >= task_id)
            {
                cout << endl << "#ERRO: Tarefa depende dela mesma ou de uma posterior a ela." << endl << endl;
                system("PAUSE");

                return -1;
            }

            task->AddDependency(tTask::task_list[number]);

            if(line[pos] != ')')
            {
                ++pos;
            }
        }
    }
    else
    {
        task->AddDependency(&cStart);
    }

    return 0;
}

struct tm TmSum(struct tm& time_1, struct tm& time_2)
{
    struct tm time_tm = {0};

    time_tm.tm_hour += time_1.tm_hour + time_2.tm_hour;
    time_tm.tm_yday += time_1.tm_yday + time_2.tm_yday;
    time_tm.tm_mon  += time_1.tm_mon  + time_2.tm_mon;
    time_tm.tm_year += time_1.tm_year + time_2.tm_year;

    return time_tm;
}

unsigned int TmToUnsigned(struct tm& time_tm)
{
    // returns the time in hours
    unsigned int time_uns = 0;

    time_uns += time_tm.tm_hour;
    time_uns += time_tm.tm_yday*8;
    time_uns += time_tm.tm_mon*20*8;
    time_uns += time_tm.tm_year*12*20*8;

    return time_uns;
}

struct tm UnsignedToTm(unsigned int time_uns)
{
    struct tm time_tm = {0};

    time_tm.tm_hour = time_uns % 8; // hours
    time_uns -= time_tm.tm_hour;
    time_uns /= 8;

    time_tm.tm_yday = time_uns % 20; // days
    time_uns -= time_tm.tm_yday;
    time_uns /= 20;

    time_tm.tm_mon = time_uns % 12; // hours
    time_uns -= time_tm.tm_mon;
    time_uns /= 12;

    time_tm.tm_year = time_uns;

    return time_tm;
}

void PrintTm(struct tm time_tm, ostream& medium)
{
    if(time_tm.tm_year != 0)
        medium << time_tm.tm_year << " anos ";
    if(time_tm.tm_mon != 0)
        medium << time_tm.tm_mon  << " meses ";
    if(time_tm.tm_yday != 0)
        medium << time_tm.tm_yday << " dias ";
    if(time_tm.tm_hour != 0)
        medium << time_tm.tm_hour << " horas ";
}

tPath::tPath()
{
    total_time = 0;
    path_list.push_back(this);
}

tPath::tPath(bool dont_push)
{
    total_time = 0;
}

tPath::tPath(tPath& rhs)
{
    total_time = rhs.total_time;
    tasks      = rhs.tasks;
}

void tPath::AddTask(tTask* task)
{
    tasks.push_back(task);
    total_time += TmToUnsigned( task->duration );
}

void tPath::Print(ostream& medium)
{
    int num_tasks = this->tasks.size();

    for(int i=0; i<num_tasks; ++i)
    {
        this->tasks[num_tasks-i-1]->PrintShort(medium);
    }

    medium << "Tempo Total: ";
    PrintTm(UnsignedToTm(total_time), medium);
    medium << endl;
}

int FindPathToStart(tTask* curr_task, tPath* curr_path)
{
    // Searches a path to the start, beginning on the passed node
    // Stops recurring when it finds a dependency on the cStart task
    int depcies_num = curr_task->depcies.size();
    tPath* last_path = new tPath(true);

    curr_path->AddTask(curr_task);

    if(depcies_num > 1) // forking here
    {
        // store path until here:
        *last_path = *curr_path;
    }

    int i = 0;

    do
    {
        if(curr_task->depcies[i] == &cStart)
        {
            return 0;
        }
        if(i>0)
        {
            tPath* dummy = (tPath*)malloc(sizeof(tPath));
            new (dummy) tPath;
            *dummy = *last_path;
            curr_path = dummy;
        }
        // it collapses when calling FP to the first dependency of the first task with forking
        FindPathToStart(curr_task->depcies[i], curr_path);

        ++i;
    }
    while( i < depcies_num );

    return 0;
}
