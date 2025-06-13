#include <iostream>
#include <mpi.h>
#include <time.h>

void task1(int rank, int size) {
	MPI_Datatype pair;
	MPI_Type_contiguous(2, MPI_INT, &pair);
	MPI_Type_commit(&pair);

	if (rank == 0)
	{
		int message[2];
		for (int i = 0; i < 2; i++)
			message[i] = rand()%10;
		MPI_Send(&message, 1, pair, 1, 0, MPI_COMM_WORLD);
	}
	if (rank == 1)
	{
		int buf[2];
		MPI_Recv(&buf, 1, pair, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		std::cout << "Received: (" << buf[0] << ":" << buf[1] << ")" << std::endl;
	}
	MPI_Type_free(&pair);
}

void task2(int rank, int size) {
	MPI_Datatype point;
	MPI_Type_contiguous(2, MPI_INT, &point);
	MPI_Type_commit(&point);

	int dimensions = 2;
	float path = 0, distance = 0, max_distance = 0;
	
	int* point_ = new int[dimensions];
	int* point2 = new int[dimensions];
	int* points = new int[size * 2];

	if (rank == 0) {
		std::cout << "Generated points:" << std::endl;
		for (int i = 0; i < size; i++) {
			points[i * 2] = rand() % 10;
			points[i * 2 + 1] = rand() % 10;
		}
	}

	MPI_Scatter(points, 1, point, point_, 2, MPI_INT, 0, MPI_COMM_WORLD);
	std::cout << "Rank: " << rank << ", point[" << point_[0] << ":" << point_[1] << "]" << std::endl;
	
	if (rank == 0) {
		for (int i = 1; i < size; i++) {
			MPI_Recv(point2, 1, point, i, i, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			distance = sqrt(pow(point2[0] - point_[0], 2) + pow(point2[1] - point_[1], 2));
			std::cout << "Rank: " << i << " distance to 0: " << distance << std::endl;
			if (distance > max_distance)
				max_distance = distance;
			path += distance;
		}
		std::cout << "Max distance: " << max_distance << ", path: " << path << std::endl;
	}
	else
		MPI_Send(point_, 1, point, 0, rank, MPI_COMM_WORLD);

	MPI_Type_free(&point);
}

void task3(int rank, int size) {
	int n = 10e7, elements = 50000;
	int* big_arr = new int[n];
	int* big_buf = new int[n];
	int* packed_arr = new int[n];
	int pos = 0;
	MPI_Datatype ten_k_ints;
	MPI_Type_contiguous(elements, MPI_INT, &ten_k_ints);
	MPI_Type_commit(&ten_k_ints);

	if (rank == 0)
	{
		for (int i = 0; i < n; i++)
			big_arr[i] = i;
		MPI_Send(big_arr, n / elements, ten_k_ints, 1, 0, MPI_COMM_WORLD);
		MPI_Pack(big_arr, n / elements, ten_k_ints, packed_arr, n * 4, &pos, MPI_COMM_WORLD);
		MPI_Send(packed_arr, pos, MPI_PACKED, 1, 0, MPI_COMM_WORLD);
	}

	if (rank == 1) {
		double time_ = MPI_Wtime();
		MPI_Recv(big_buf, n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		double time2 = MPI_Wtime();
		std::cout << "Spent without packing:\t" << time2 - time_ << std::endl;

		time_ = MPI_Wtime();
		MPI_Recv(big_buf, n * 4, MPI_PACKED, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		MPI_Unpack(big_buf, n * 4, &pos, big_buf, n / 50000, MPI_INT, MPI_COMM_WORLD);
		time2 = MPI_Wtime();
		std::cout << "Spent with packing:\t" << time2 - time_ << std::endl;
	}

}

int main(int argc, char** argv) {
	MPI_Init(&argc, &argv);
	srand(time(NULL));

	int rank, size;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);

	//task1(rank, size);
	//task2(rank, size);
	task3(rank, size);

	MPI_Finalize();
	return 0;
}
