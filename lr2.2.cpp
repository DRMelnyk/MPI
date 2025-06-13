#include <iostream>
#include <stdlib.h>
#include <mpi.h>

int const count = 5;
int rank, size;
int messageInt = 0;


inline void SendMessageInt(int dest, int* message, MPI_Request* req, MPI_Status* stat)
{
	MPI_Send(message, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
}

inline void RecieveMessageInt(MPI_Request* req, MPI_Status* stat)
{
	int ret = MPI_Irecv(&messageInt, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, req);
	MPI_Wait(req, stat);
	std::cout << "[PROC#" << rank << "] Message from process " << stat->MPI_SOURCE << ": " << messageInt << std::endl;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	MPI_Request request;
	MPI_Status stat;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	int sendto = rank + 1 == size ? 0 : rank + 1;

	for (int i = 0; i < 100; i++) {
		if (rank == 0) {
			messageInt = 0;
			SendMessageInt(sendto, &messageInt, &request, &stat);
			RecieveMessageInt(&request, &stat);
		}
		else {
			RecieveMessageInt(&request, &stat);
			messageInt++;
			SendMessageInt(sendto, &messageInt, &request, &stat);
		}
	}

	MPI_Finalize();
	return 0;
}

