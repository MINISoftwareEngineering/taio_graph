#pragma once
#include <config.h>
#include <graph_manager/graph_manager.h>
#include <limits>
#include <future>
#include <thread>
#include <chrono>

inline std::ostream& operator<<(std::ostream& os, const graph_extention_t& extention)
{
    if (extention.empty())
    {
        os << "|   [] \n";
        return os;
    }

    int i = 0;
    os << "|   [ ";
    for each (edge e in extention)
    {
        if (i % 6 == 0)
            os << "\n|     ";
        os << e.start << " -> " << e.end;
        if (i++ != extention.size() - 1)
            os << ", ";
    }
    os << "\n|   ] \n";
    return os;
}

inline std::ostream& operator<<(std::ostream& os, const path_t& path)
{
    os << "[ ";
    for (size_t i = 0; i < path.size(); ++i)
    {
        os << path[i];
        os << " -> ";
        if (i == path.size() - 1)
            os << path[0];
    }
    os << " ]";
    return os;
}

struct ConsoleManager
{
    void clear()
    {
        #ifdef _WIN32
            system("cls");
        #else
            system("clear");
        #endif
    }

    void write(std::string text)
    {
        std::cout << text;
    }

    void write(const graph_extention_t& graph_extention)
    {
        std::cout << graph_extention;
    }

    void waitForEnter()
    {
            std::cout << "Nacisnij enter, aby kontynuowac...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void waitForEnterToClose()
    {
        std::cout << "Nacisnij enter, aby zakonczyc...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    void listGraphsSizes(std::vector<GraphData>& graphs_data)
    {
        write("Graphs sizes: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
            write("|- graph " + std::to_string(i) + " size: "
                + std::to_string(graph_manager.getGraphSize(graphs_data[i])) + "\n");
    }

    void listPreciseGraphsHamiltonCycleExtentions(std::vector<GraphData>& graphs_data, bool long_output)
    {
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graphs_data[i].getNodesCount() > 8)
                write("Liczba wierzcholkow > 8 - graf pominiety \n");
            else
            {
                if (graph_data.isPreciseHamiltonCycleGraphExtentionAssigned())
                {
                    int graph_size = graph_manager.getGraphSize(graph_data);

                    std::string precise_hamilton_cycles_count = std::to_string(graph_data.getPreciseHamiltonCycleCount());
                    //std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
                    //std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);
                    std::chrono::milliseconds duration = graph_data.getPreciseHamiltonCycleTime();

                    //write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                    write("|- czas wykonania: " + std::to_string(duration.count()) + "ms\n");
                    if (!graph_data.getPreciseHamiltonCycles().empty())
                    {
                        write("|- najmniejsze rozszerzenie (liczba krawedzi: " + std::to_string(graph_data.getPreciseHamiltonCycleGraphExtension().size()) + "): \n| [");

                        std::vector<std::pair<int, int>> precise_hamilton_cycle_graph_extention = graph_data.getPreciseHamiltonCycleGraphExtension();
                        for (int j = 0; j < precise_hamilton_cycle_graph_extention.size(); j++)
                        {
                            std::pair<int, int> current_edge = precise_hamilton_cycle_graph_extention[j];
                            write(std::to_string(current_edge.first) + " -> " + std::to_string(current_edge.second));
                            if (j < precise_hamilton_cycle_graph_extention.size() - 1)
                                write(", ");
                        }
                        write("]\n");
                        write("|- liczba cykli Hamiltona (po dodaniu minimalnego rozszerzenia): " + precise_hamilton_cycles_count + " \n");
                    }
                    else write("|- nie znaleziono rozszerzenia do cyklu Hamiltona\n");
                }
                else
                    write("|- Blad - nie znaleziono grafu \n");
            }
            
        }
    }

    void writeGraphHamiltonCycleExtention(GraphData& graph_data, bool long_output)
    {
        //write("Aproksymacja algorytmu znajdującego \nminimalne rozszerzenie grafu do grafu zawierającego cykl Hamiltona \noraz liczbę cykli Hamiltona: \n");

        if (graph_data.isHamiltonCycleGraphExtentionAssigned())
        {
            int graph_size = graph_manager.getGraphSize(graph_data);

            std::string hamilton_cycles_count = std::to_string(graph_data.getHamiltonCycles().size());
            std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
            std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
            std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);
            std::string note = "";
            if (graph_data.getHamiltonCycleGraphExtentionSize() > 0)
                note = " (dla grafu z dodanymi krawedzami z najmniejszego rozszerzenia)";

            write("Wyniki: \n");

            /*if (graph_data.getNodesCount() <= 32)
            {


                write("|  |- macierz sąsiedztwa: \n");

                for (int j = 0; j < graph_data.getNodesCount(); j++)
                {
                    write("|  |   [");
                    for (int k = 0; k < graph_data.getNodesCount(); k++)
                    {
                        if (graph_manager.hasEdge(graph_data, j, k))
                        {
                            write(" 1 ");
                        }
                        else write(" 0 ");
                    }
                    write("] \n");
                }
            }
            else
                write("|  |- macierz sąsiedztwa: (nie wyświetlona, za dużo wierzchołków) \n");*/
            if (long_output && graph_data.getNodesCount() > 1)
                write("|- czas szukania wyniku: " + full_execution_time + " ms  (minimalne rozszerzenie: " + find_extention_execution_time + " ms, liczba cykli: " + find_cycles_execution_time + " ms) \n");
            else
                write("|- czas szukania wyniku: " + full_execution_time + " ms \n");

            if (graph_data.getNodesCount() == 1)
            {
                write("|- nie znaleziono rozszerzenia do cyklu Hamiltona\n");
            }
            else
            {
                write("|- najmniejsze rozszerzenie (liczba krawedzi: " + std::to_string(graph_data.getHamiltonCycleGraphExtentionSize()) + "): \n");
                write(graph_data.getHamiltonCycleGraphExtention());

                GraphData graph_data_ex = graph_data;
                if (long_output)
                {
                    if (graph_data.getNodesCount() <= 32)
                    {
                        write("|  rozszerzenie zaznaczone na macierzy sasiedztwa: \n");


                        for each (edge e in graph_data_ex.getHamiltonCycleGraphExtention())
                        {
                            graph_manager.addEdge(graph_data_ex, e);
                        }

                        for (int j = 0; j < graph_data.getNodesCount(); j++)
                        {
                            write("|   [");
                            for (int k = 0; k < graph_data_ex.getNodesCount(); k++)
                            {
                                if (graph_manager.hasEdge(graph_data, j, k))
                                {
                                    write(" 1 ");
                                }
                                else if (graph_manager.hasEdge(graph_data_ex, j, k))
                                {
                                    write("(1)");
                                }
                                else write(" 0 ");
                            }
                            write("] \n");
                        }
                    }
                    else
                        write("|  rozszerzenie zaznaczone na macierzy sasiedztwa: (nie wyswietlone, za duzo wierzchołkow) \n");
                }

                write("|- liczba cykli hamiltona" + note + ": " + hamilton_cycles_count + " \n");

                if (long_output)
                {
                    if (graph_data.getNodesCount() <= 32)
                    {
                        std::string no_cycles = "";
                        if (graph_data.getHamiltonCycles().size() == 0)
                            no_cycles = "brak cykli ";

                        write("|  cykle zaznaczone na macierzy sadziedztwa: " + no_cycles + "\n");

                        int c = 1;
                        for each (path_t cycle in graph_data.getHamiltonCycles())
                        {
                            write("|  |- cykl " + std::to_string(c++) + ": ");
                            std::cout << cycle;
                            write("\n");
                            for (int j = 0; j < graph_data.getNodesCount(); j++)
                            {
                                write("|  |  |   [");
                                for (int k = 0; k < graph_data.getNodesCount(); k++)
                                {
                                    if (graph_manager.hasEdge(graph_data_ex, j, k))
                                    {
                                        auto it = std::find(cycle.begin(), cycle.end(), j);
                                        int index = std::distance(cycle.begin(), it);

                                        if (cycle[(index + 1) % graph_data.getNodesCount()] == k)
                                            write("(1)");
                                        else
                                            write(" 1 ");
                                    }
                                    else
                                        write(" 0 ");
                                }
                                write("] \n");
                            }
                        }
                    }
                    else
                        write("|  cykle zaznaczone na macierzy sadziedztwa: (nie wyswietlone, za duzo wierzcholkow) \n");
                }
                else if (!graph_data.getHamiltonCycles().empty())
                {
                    write("|- Przykladowy znaleziony cykl:\n|   ");
                    for each (path_t path in graph_data.getHamiltonCycles())
                    {
                        printCycle(path);
                        break;
                    }
                }
            }
        }
        else
            write("Wyniki: Wykonanie nie powiodlo sie \n");
    
    }

    void listGraphsHamiltonCycleExtentions(std::vector<GraphData>& graphs_data)
    {
        write("Approximate Graphs Hamilton Cycle extentions: \n");
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graph_data.isHamiltonCycleGraphExtentionAssigned())
            {
                int graph_size = graph_manager.getGraphSize(graph_data);

                std::string hamilton_cycles_count = std::to_string(graph_data.getHamiltonCycles().size());
                std::string full_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs + graph_data.findAllHamiltonCyclesExecutionTimeMs);
                std::string find_cycles_execution_time = std::to_string(graph_data.findAllHamiltonCyclesExecutionTimeMs);
                std::string find_extention_execution_time = std::to_string(graph_data.findMinimumExtentionForHamiltonCycleExecutionTimeMs);

                write("|- graph " + std::to_string(i) + ": \n");
                write("|  |- finding full solution time: " + full_execution_time + " ms  (finding minumum graph: " + find_extention_execution_time + " ms, finding hamilton cycles: " + find_cycles_execution_time + " ms) \n");
                write("|  |- hamilton cycles: " + hamilton_cycles_count + " \n");
                write("|  |- smallest extention (edges count: " + std::to_string(graph_data.getHamiltonCycleGraphExtentionSize()) + "): \n");
                write(graph_data.getHamiltonCycleGraphExtention());
            }
            else
                write("|- graph " + std::to_string(i) + ": Finding failed \n");
        }
    }


    void listLongestCycles(std::vector<GraphData>& graphs_data)
    {
        for (int i = 0; i < graphs_data.size(); ++i)
        {
            GraphData& graph_data = graphs_data[i];

            if (graphs_data[i].getNodesCount() > 8)
                write("|- graf " + std::to_string(i) + ": liczba wierzcholkow > 8 - pominieto \n");
            else
            {
                if (graph_data.isLongestCyclesAssigned())
                {
                    std::chrono::milliseconds duration = graph_data.getLongestCyclesTime();
                    write("|  |- czas wykonywania funkcji: " + std::to_string(duration.count()) + "ms\n");
                    std::vector<std::vector<int>> longest_cycles = graph_data.getLongestCycles();
                    if (longest_cycles.size() == 0)
                    {
                        write("|  |- Nie znaleziono cykli!\n");
                    }
                    else
                    {
                        std::vector<int> first_longest_cycle = longest_cycles[0];
                        write("|  |- dlugosc najdluzszego cyklu: " + std::to_string(first_longest_cycle.size()) + " (liczba cykli: " + std::to_string(longest_cycles.size()) + ")\n");
                    }
                }
                else
                    write("|- graf " + std::to_string(i) + ": Obliczenia nie powiodly sie \n");
            }

        }
    }

    void listGraphsLongestCycles(std::vector<GraphData>& graphs_data)
    {
        write("Najdluzsze cykle (algorytm aproksymacyjny): \n");
        for (int i = 0; i < graphs_data.size(); ++i) {
            GraphData& graph_data = graphs_data[i];
            int graph_size = graph_manager.getGraphSize(graph_data);
            std::chrono::milliseconds duration = graph_data.getApproximateLongestCyclesTime();
            int approximate_longest_cycles_count = graph_data.getApproximateLongestCycles().size();
            std::vector<std::vector<int>> approximate_longest_cycles_values = graph_data.getApproximateLongestCycles();
            write("|- Rozmiar grafu: " + std::to_string(graph_size) + ": \n");
            write("|- czas wykonania: " + std::to_string(duration.count()) + "ms\n");
            if (approximate_longest_cycles_count > 0)
            {
                write("|- Najdluzsze cykle (aproksymacyjny) (dlugosc najdluzszego cyklu: " + std::to_string(approximate_longest_cycles_values[0].size()) + ") : \n");
                /*for (int j = 0; j < approximate_longest_cycles_count; j++)
                {
                    write("|- cykl numer " + std::to_string(j) + ": \n");
                    printCycle(approximate_longest_cycles_values[j]);
                }*/
            }
            else
            {
                write("|- Najdluzsze cykle (aproksymacyjny): ");
                write("  Nie znaleziono cykli!\n");
            }
        }
    }
    void printCycle(const std::vector<int>& cycle)
    {
        if (cycle.empty()) {
            write("Cykl jest pusty.\n");
            return;
        }
        write(std::to_string(cycle[0]));
        for (int i = 1; i < cycle.size(); i++) {
            write(" -> " + std::to_string(cycle[i]));
        }
        write(" -> " + std::to_string(cycle[0]));

        write("\n");
    }


    void writeDistanceBetweenGraphs(std::vector<GraphData> graphs, int index1, int index2)
    {
        write("Calculating accurate metric between graph "
            + std::to_string(index1) + "(" + std::to_string(graphs.at(index1).getNodesCount()) + ")" + " and graph "
            + std::to_string(index2) + "(" + std::to_string(graphs.at(index2).getNodesCount()) + ")\n");

        write("Finished calcualting metric distance between graphs "
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId())
            + ".\nThe result is "
            + std::to_string(graph_manager.getMetricDistance(graphs[index1], graphs[index2])) + " \n");

    }


    void writeApproximateDistanceBetweenGraph(std::vector<GraphData> graphs, int index1, int index2) {
        write("Calculating approximate metric between graph "
            + std::to_string(index1) + "(" + std::to_string(graphs.at(index1).getNodesCount()) + ")" + " and graph "
            + std::to_string(index2) + "(" + std::to_string(graphs.at(index2).getNodesCount()) + ")\n");

        write("Approximate metric distance between graphs "
            + std::to_string(graphs[index1].getId()) + " and " + std::to_string(graphs[index2].getId()) 
            +".\nThe result is "
            + std::to_string(graph_manager.tryGetMetricDistance(graphs[index1], graphs[index2])) + " \n");
    }

    void writeGraphsIndiciesWithSizes(std::vector<GraphData> graphs) {

        write("List of available graphs loaded from data files, the output format is: index(size)\n");
        int index = 0;
        for (auto& graph : graphs) {
            write(std::to_string(index++) + "(" + std::to_string(graph.getNodesCount()) + ") ");
        }
        write("\n");
    }

    void writeGraphInConsole(GraphData graph) 
    {
        for (auto& row : graph.adjacency_matrix_representation) {
            std::cout << "[";
            for (auto value : row) {
                std::cout << " " << value << " ";
            }
            std::cout << "]";
            std:: cout << std::endl;
        }
    }
    void writeCycleOnGraph(GraphData graph, std::vector<int> cycle) 
    {
        // Get the size of the adjacency matrix
        int n = graph.adjacency_matrix_representation.size();

        // Create a copy of the adjacency matrix for modification (printing only)
        std::vector<std::vector<std::string>> formatted_matrix(n, std::vector<std::string>(n, ""));

        // Populate the formatted_matrix with original values
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                formatted_matrix[i][j] = std::to_string(graph.adjacency_matrix_representation[i][j]);
            }
        }

        // Highlight the edges of the cycle in the formatted matrix
        for (size_t i = 0; i < cycle.size(); ++i) {
            int from = cycle[i];
            int to = cycle[(i + 1) % cycle.size()]; // Wrap around to form a cycle

            if (graph.adjacency_matrix_representation[from][to] != 0) {
                formatted_matrix[from][to] = "(" + formatted_matrix[from][to] + ")";
            }
        }

        // Print the formatted matrix
        std::cout << "Macierz sasiedztwa z zaznaczonym cyklem:\n";
        for (const auto& row : formatted_matrix) {
            std::cout << "[";
            for (const auto& cell : row) {
                if (cell.length() > 1)
                    std::cout << cell;
                else 
                    std::cout << " " << cell << " ";
            }
            std::cout << "]";
            std::cout << "\n";
        }
    }
private:
    GraphManager graph_manager;
};

