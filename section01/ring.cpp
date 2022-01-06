#include <iostream>
#include <assert.h>
#include <math.h>
#include <mpi.h>

using namespace std;

void bidirectional_ring(size_t n_nodes, int rank, MPI_Comm GRID_COMM_WORLD);

int main(int argc, char *argv[])
{
    // ---- Read n_iterations from command line ---- //
    size_t n_iterations = atoi(argv[1]);

    // ---- Declare variables ---- //
    int n_nodes, node_rank;
    int n_dimensions = 1;
    int periodic = 1;
    bool reorder = true;

    // ---- Initialize MPI ---- //
    MPI_Init(&argc, &argv);
    MPI_Comm GRID_COMM_WORLD;
    MPI_Comm_size(MPI_COMM_WORLD, &n_nodes);
    MPI_Cart_create(MPI_COMM_WORLD,
                    n_dimensions,
                    &n_nodes,
                    &periodic,
                    reorder,
                    &GRID_COMM_WORLD);
    MPI_Comm_rank(GRID_COMM_WORLD, &node_rank);

    // ---- Run the bidirectional ring ---- //
    double times[n_iterations];
    for (size_t i = 0; i < n_iterations; i++)
    {
        times[i] = MPI_Wtime();
        bidirectional_ring(n_nodes, node_rank, GRID_COMM_WORLD);
        times[i] = (MPI_Wtime() - times[i]);
    }

    // ---- Compute average time ---- //
    double avg_time = 0;
    for (size_t i = 0; i < n_iterations; i++)
    {
        avg_time += times[i];
    }
    avg_time /= n_iterations;

    // ---- Print results from process 0 ---- //
    if (node_rank == 0)
    {
        std::cout << n_nodes
                  << "," << avg_time
                  << std::endl;
    }

    // ---- Clean up ---- //
    MPI_Finalize();

    return 0;
}

void bidirectional_ring(size_t n_nodes, int rank, MPI_Comm GRID_COMM_WORLD)
{
    // ---- Get the rank of the left and right neighbors ---- //
    size_t direction = 0;
    size_t displacement = 1;
    int left_node, right_node;

    MPI_Cart_shift(GRID_COMM_WORLD,
                   direction,
                   displacement,
                   &left_node,
                   &right_node);

    // ---- Declare MPI variables ---- //
    MPI_Request requests[4];
    MPI_Status status[4];

    // ---- Declare variables ---- //
    size_t counter = 0;
    int left_send_msg, right_send_msg;
    int left_rec_msg = 0;
    int right_rec_msg = 0;

    // ---- Send and receive messages ---- //
    for (size_t i = 0; i < n_nodes; ++i)
    {
        left_send_msg = right_rec_msg - rank;
        right_send_msg = left_rec_msg + rank;

        // ---- Non-blocking send ---- //
        MPI_Isend(&left_send_msg, 1, MPI_INT, left_node, rank, GRID_COMM_WORLD, &requests[0]);
        MPI_Isend(&right_send_msg, 1, MPI_INT, right_node, rank, GRID_COMM_WORLD, &requests[1]);

        // ---- Non-blocking receive ---- //
        MPI_Irecv(&left_rec_msg, 1, MPI_INT, left_node, MPI_ANY_TAG, GRID_COMM_WORLD, &requests[2]);
        MPI_Irecv(&right_rec_msg, 1, MPI_INT, right_node, MPI_ANY_TAG, GRID_COMM_WORLD, &requests[3]);

        // ---- Wait for all messages to be sent and received ---- //
        MPI_Waitall(4, requests, status);

        // ---- Increment counter of received messages ---- //
        counter += 2;

        // ---- Check if received tags are proportional to the rank of the sender ---- //
        int left_rec_tag = status[2].MPI_TAG;
        int right_rec_tag = status[3].MPI_TAG;
        int left_rec_rank = status[2].MPI_SOURCE;
        int right_rec_rank = status[3].MPI_SOURCE;

        int left_tag_test = (left_rec_rank == 0) ? left_rec_tag : (left_rec_tag % left_rec_rank);
        int right_tag_test = (right_rec_rank == 0) ? right_rec_tag : (right_rec_tag % right_rec_rank);
        assert(left_tag_test == 0);
        assert(right_tag_test == 0);

        // ---- Print results on last iteration ---- //
        if (i == n_nodes - 1)
        {
            std::cout << "I am process " << rank
                      << " and i have received " << counter
                      << " messages. My final messages have tag ("
                      << status[2].MPI_TAG << "," << status[3].MPI_TAG
                      << ") and value (" << left_rec_msg
                      << ", " << right_rec_msg << ")." << std::endl;
        }
    }

    return;
}
