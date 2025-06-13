#include <iostream>
#include <mpi.h>
#include <time.h>

#define ARR_SIZE 1000

int* CreateRandomArray(int x, int y) {
	srand(time(NULL));
	int* outRand = (int*)malloc(sizeof(int) * x * y);
	int i;
	for (i = 0; i < x * y; i++) {
		outRand[i] = (10*rand() / RAND_MAX);
	}
	return outRand;
}

void MainCollective(int argc, char** argv)
{
	int numtasks, rank;
	int* a = 0;
	int* b = 0;
	int* c = 0;
	int i, j, k;

	double startTime = 0.0;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Создание декартовой топологии
	int dims[2] = { 0, 0 };
	MPI_Dims_create(numtasks, 2, dims);
	int periods[2] = { 0, 0 };
	int reorder = 0;
	MPI_Comm cart_comm;
	MPI_Cart_create(MPI_COMM_WORLD, 2, dims, periods, reorder, &cart_comm);
	MPI_Comm_size(cart_comm, &numtasks);
	MPI_Comm_rank(cart_comm, &rank);

	if (rank == 0)
	{
		a = CreateRandomArray(ARR_SIZE, ARR_SIZE);
		b = CreateRandomArray(ARR_SIZE, ARR_SIZE);
		c = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
	}

	MPI_Barrier(cart_comm);
	if (rank == 0)
	{
		startTime = MPI_Wtime();
	}

	if (rank != 0)
	{
		b = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
		c = (int*)malloc(sizeof(int) * ARR_SIZE * ARR_SIZE);
	}

	MPI_Bcast(b, ARR_SIZE * ARR_SIZE, MPI_FLOAT, 0, MPI_COMM_WORLD);
	int* elementsPerProc = (int*)malloc(sizeof(int) * numtasks);
	int* displacements = (int*)malloc(sizeof(int) * numtasks);

	for (i = 0; i < numtasks; i++)
	{
		elementsPerProc[i] = ARR_SIZE / numtasks;
		elementsPerProc[i] = elementsPerProc[i] * ARR_SIZE;
	}

	int unassignedRows = ARR_SIZE % numtasks;
	if (unassignedRows != 0)
	{
		for (i = 0; i < unassignedRows; i++)
			elementsPerProc[i] += ARR_SIZE;

	}

	displacements[0] = 0;
	for (i = 1; i < numtasks; i++)
		displacements[i] = displacements[i - 1] + elementsPerProc[i - 1];

	int* local_a = (int*)malloc(sizeof(int) * elementsPerProc[rank]);
	int* local_c = (int*)malloc(sizeof(int) * elementsPerProc[rank]);

	MPI_Scatterv(a, elementsPerProc, displacements, MPI_INT, local_a, elementsPerProc[rank], MPI_INT, 0, MPI_COMM_WORLD);
	int rows = elementsPerProc[rank] / ARR_SIZE;
	int sum = 0;
	for (i = 0; i < rows; i++)
	{
		for (j = 0; j < ARR_SIZE; j++)
		{
			sum = 0;
			for (k = 0; k < ARR_SIZE; k++)
			{
				sum += local_a[i * ARR_SIZE + k] * b[k * ARR_SIZE + j];
			}
			local_c[i * ARR_SIZE + j] = sum;
		}
	}

	MPI_Gatherv(local_c, elementsPerProc[rank], MPI_INT, c, elementsPerProc, displacements, MPI_INT, 0, MPI_COMM_WORLD);

	if (rank == 0)
	{
		double totalTime = MPI_Wtime() - startTime;
		printf("Total run time = %f\n", totalTime);
	}

	free(b);
	free(c);
	free(elementsPerProc);
	free(displacements);
	free(local_a);
	free(local_c);
	MPI_Finalize();
}

int main(int argc, char** argv)
{
	MainCollective(argc, argv);
	return 0;
}

//////mpiexec -n 20 "D:\Учёба\4й курс\VII_term\2_Теорія-розподілених-систем\ЛР4\lr4\x64\Debug\lr4.exe"
//
//#include <mpi.h>
//#include <iostream>
//
//#define MAXPROC 128
//#define MAXEDGES 512
//
//void create_cartesian_topology(int size, int dimensions[2], MPI_Comm* cartesian_comm) {
//    int periods[2] = { 0, 0 }; // periodicity
//    MPI_Cart_create(MPI_COMM_WORLD, 2, dimensions, periods, 0, cartesian_comm);
//}
//
//void create_graph_topology(int size, MPI_Comm* graph_comm) {
//    int index[MAXPROC], edges[MAXEDGES];
//
//    for (int i = 0; i < size; i++) index[i] = size + i - 1;
//    for (int i = 0; i < size - 1; i++) {
//        edges[i] = i + 1;
//        edges[size + i - 1] = 0;
//    }
//
//    MPI_Graph_create(MPI_COMM_WORLD, size, index, edges, 0, graph_comm);
//}
//
//int main(int argc, char** argv) {
//    MPI_Init(&argc, &argv);
//
//    int size, rank;
//    MPI_Comm_size(MPI_COMM_WORLD, &size);
//
//    // Create cartesian topology
//    MPI_Comm cartesian_comm;
//    int dimensions[2] = { 4, 5 };
//    create_cartesian_topology(size, dimensions, &cartesian_comm);
//    MPI_Comm_rank(cartesian_comm, &rank);
//
//    // Create graph topology
//    MPI_Comm graph_comm;
//    create_graph_topology(size, &graph_comm);
//    MPI_Comm_rank(graph_comm, &rank);
//
//    // Compare performance
//    int num_iterations = 10;
//    double start_time, end_time;
//
//    // Cartesian topology
//    if(rank==0) 
//        start_time = MPI_Wtime();
//    
//    {
//        for (int i = 0; i < num_iterations; i++) {
//            int r, s;
//            MPI_Comm_rank(cartesian_comm, &r);
//            MPI_Comm_size(cartesian_comm, &s);
//            int dest = (r + 1) % s;
//            int source = (r - 1 + s) % s;
//            int send_data = r;
//            int recv_data;
//            MPI_Sendrecv(&send_data, 1, MPI_INT, dest, 0, &recv_data, 1, MPI_INT, source, 0, cartesian_comm, MPI_STATUS_IGNORE);
//        }
//    }
//    if (rank == 0){
//        end_time = MPI_Wtime();
//        std::cout << "Cartesian topology time: " << (end_time - start_time) / num_iterations << " seconds" << std::endl;
//    }
//
//    // Graph topology
//    if(rank == 0) 
//        start_time = MPI_Wtime();
//    {
//        for (int i = 0; i < num_iterations; i++) {
//            int r, s;
//            MPI_Comm_rank(graph_comm, &r);
//            MPI_Comm_size(graph_comm, &s);
//            int dest = (r + 1) % s;
//            int source = (r - 1 + s) % s;
//            int send_data = r;
//            int recv_data;
//            MPI_Sendrecv(&send_data, 1, MPI_INT, dest, 0, &recv_data, 1, MPI_INT, source, 0, graph_comm, MPI_STATUS_IGNORE);
//        }
//    }
//    if(rank == 0) {
//        end_time = MPI_Wtime();
//        std::cout << "Graph topology time: " << (end_time - start_time) / num_iterations << " seconds" << std::endl;
//    }
//
//    MPI_Finalize();
//    return 0;
//}