void task_1(int size, int rank) {
    MPI_Comm comm1;
    MPI_Comm_dup(MPI_COMM_WORLD, &comm1);

    MPI_Comm comm2;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 2, rank, &comm2);

    MPI_Comm comm3;
    MPI_Comm_split(MPI_COMM_WORLD, (rank + 1) % 2, rank, &comm3);

    MPI_Group group1, group2, group3;
    MPI_Comm_group(comm1, &group1);
    MPI_Comm_group(comm2, &group2);
    MPI_Comm_group(comm3, &group3);

    int result;
    MPI_Group_compare(group1, group2, &result);
    if (result == MPI_IDENT) {
        std::cout << "The groups of the first communicator are identical." << std::endl;
    }
    else {
        std::cout << "The groups of the first communicator are not identical." << std::endl;
    }

    int group1_size, group2_size, group3_size;
    MPI_Group_size(group1, &group1_size);
    MPI_Group_size(group2, &group2_size);
    MPI_Group_size(group3, &group3_size);

    std::cout << "Process " << rank << " in group1 has " << group1_size << " processes." << std::endl;
    std::cout << "Process " << rank << " in group2 has " << group2_size << " processes." << std::endl;
    std::cout << "Process " << rank << " in group3 has " << group3_size << " processes." << std::endl;
}

void task_2(int size, int rank) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 3);
    int color = dis(gen);

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

    int new_rank;
    MPI_Comm_rank(new_comm, &new_rank);

    if (color == 0) {
        std::cout << "Process " << rank << " belongs to the first group." << std::endl;
    }
}

void task_3(int size, int rank) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);
    int color = dis(gen);

    MPI_Comm new_comm;
    MPI_Comm_split(MPI_COMM_WORLD, color, rank, &new_comm);

    int new_rank;
    MPI_Comm_rank(new_comm, &new_rank);

    if (color == 0) {
        std::cout << "Process " << rank << " belongs to the first group." << std::endl;
    }

}


