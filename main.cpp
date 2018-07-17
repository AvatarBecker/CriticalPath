#include <cstdlib>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>
#include "tTask.hpp"
using namespace std;

#define projeto "Projeto"

int main()
{
    size_t cursor;
    string line;
    string project_name;

    cout << "### Caminho Critico ### \a" << endl << endl;
    cout << "Este programa le um arquivo de nome \"entradas_projeto.txt\" " << endl;
    cout << "contendo as tarefas de um projeto, e cria um arquivo de nome " << endl;
    cout << "\"Caminho Critico Proj XXX\" com a lista de tarefas do caminho " << endl;
    cout << "critico do projeto, juntamente com sua duracao total minima." << endl << endl;

    cout << "Exemplo de arquivo de entrada:" << endl << endl;

    cout << "Entradas - Projeto XXX" << endl;
    cout << "Entrada.1 - Reuniao com Cliente (10h)" << endl;
    cout << "Entrada.2 - Escopo do Projeto (1d)(D1)" << endl;
    cout << "Entrada.3 - Planejamento das Etapas (5d)(D1,2)" << endl;
    cout << "Entrada.4 - Etapa 1 (1a5m)(D3)" << endl;
    cout << "Entrada.5 - Etapa 2 (1a3m10d6h)(D4)" << endl;
    cout << "Entrada.6 - Finalizacao (5h)(D5)" << endl << endl;

    cout << "As atividades devem estar em ordem. Uma atividade \"Fim\" deve estar presente," << endl;
    cout << "mesmo que de duracao nula (0h)." << endl;

    cout << "Por favor, certifique-se que o arquivo \"entradas_projeto.txt\" existe " << endl;
    cout << "no mesmo diretorio deste executavel, e que ele esteja no formato acima. " << endl;
    cout << "Caso contrario, o programa pode apresentar comportamento imprevisivel " << endl;
    cout << "e todos sofrerao as consequencias." << endl << endl;

    system("PAUSE");
    cout << endl;

    // Read File
    ifstream in_file("entradas_projeto.txt");

    // Check if file is open:
    if(!in_file.is_open())
    {
        cout << endl << "#ERRO: Arquivo inexistente ou incompativel." << endl << endl;
        system("PAUSE");

        return 0;
    }

    // Get project name:

    // get next line until it isn't empty
    do
    {
        getline(in_file,line);
    }
    while( ( line.find_first_not_of(" \t\n\r") == string::npos || line.empty() ) && !in_file.eof());

    // Check if file has ended:
    if(in_file.eof())
    {
        cout << endl << "#ERRO: Arquivo vazio." << endl << endl;
        system("PAUSE");

        return 0;
    }

    cursor = line.find(projeto);
    cursor = line.find_first_not_of(' ', cursor + sizeof(projeto));

    project_name.assign(line, cursor, line.size() - cursor + 1 );
    cout << "Projeto " << project_name << ":" << endl << endl;

    int task_it = 1;
    while(!in_file.eof())
    {
        // Parse for task characteristics

        // get next line until it isn't empty
        do
        {
            getline(in_file,line);
        }
        while( ( line.find_first_not_of(" \t\n\r") == string::npos || line.empty() ) && !in_file.eof());

        if( !( line.find_first_not_of(" \t\n\r") == string::npos || line.empty() ) )
        {
            // ParseForTaskData(line, task_list);
            if(ParseForTaskData(line))
            {
                cout << endl << "#ERRO: Uma ou mais tarefas nao puderam ser parseadas." << endl << endl;
                system("PAUSE");

                return 0;
            }

            tTask::task_list[task_it]->Print(cout);
            ++task_it;
        }
        cout << endl;
    }

    cout << endl;

    in_file.close();

    tPath* curr_path = new tPath;
    int num_tasks = tTask::task_list.size();

    // Search for all paths
    FindPathToStart(tTask::task_list[num_tasks-1], curr_path);

    int num_paths = tPath::path_list.size();
    for(int i=0; i<num_paths; ++i)
    {
        cout << "Path " << i <<":" << endl;
        tPath::path_list[i]->Print(cout);
        cout << endl;
    }

    // Find longest path:
    tPath* longest_path;
    longest_path = *max_element(tPath::path_list.begin(), tPath::path_list.end(), ShorterThan);

    for(int i=0; i < tPath::path_list.size(); ++i)
    {
        if(tPath::path_list[i]->total_time == longest_path->total_time)
        {
            cout << "Caminho Critico:" << endl;
            tPath::path_list[i]->Print(cout);
            cout << endl;
        }
    }

    // Write stuff to output file:
    string out_file_name = "Caminho Critico - Projeto ";

    out_file_name.append(project_name);
    out_file_name.append(".txt");
    ofstream out_file(out_file_name.c_str());

    out_file << "Caminho Critico - Projeto " << project_name << endl << endl;

    for(int i=0; i < tPath::path_list.size(); ++i)
    {
        if(tPath::path_list[i]->total_time == longest_path->total_time)
        {
            out_file << "Caminho Critico:" << endl;
            tPath::path_list[i]->Print(out_file);
            out_file << endl;
        }
    }

    out_file << "Data: ";

    time_t now        = time(NULL);
    struct tm* now_tm = localtime(&now);

    out_file << now_tm->tm_mday << "/"
             << now_tm->tm_mon + 1  << "/"
             << now_tm->tm_year + 1900;

    out_file.close();

    system("PAUSE");

    return 0;
}
