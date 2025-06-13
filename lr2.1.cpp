#include <iostream>
#include <stdlib.h>
#include <mpi.h>

int const count = 5;

inline void SendMessage(int dest, char* message, MPI_Request* req, MPI_Status* stat)
{
	MPI_Isend(message, count, MPI_CHAR, dest, 0, MPI_COMM_WORLD, req);
	MPI_Wait(req, stat);
}

inline void RecieveMessage(MPI_Request* req, MPI_Status* stat)
{
	char* recvBuf = new char[count];
	int ret = MPI_Irecv(recvBuf, count, MPI_CHAR,
		MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, req);
	MPI_Wait(req, stat);
	std::cout << "Message from process " << stat->MPI_SOURCE << ": "
		<< recvBuf << std::endl;
	delete[] recvBuf;
}

int main(int argc, char** argv)
{
	MPI_Init(&argc, &argv);

	int rank;
	char pingBuf[] = "Ping";
	char pongBuf[] = "Pong";
	MPI_Request request;
	MPI_Status stat;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double startTime = MPI_Wtime();
	double currentTime = 0.0;

	if (rank) {
		while ((currentTime - startTime) < 1) {
			currentTime = MPI_Wtime();
			int flag = 0;
			MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &flag, &stat);
			if (flag) {
				RecieveMessage(&request, &stat);
				SendMessage(0, pongBuf, &request, &stat);
			}
		}
	}
	else {
		while ((currentTime - startTime) < 1) {
			currentTime = MPI_Wtime();
			SendMessage(1, pingBuf, &request, &stat);
			RecieveMessage(&request, &stat);
		}
	}

	MPI_Finalize();
	return 0;
}

