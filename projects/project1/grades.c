/* Name: Shiva Sharma
 * UID: 117912678
 * UMD Directory ID: ssharm08 */

/* library imports */
#include <math.h>
#include <stdio.h>

/* constant definitions */
#define TRUE 1
#define FALSE 0
#define MAX_NUM_ASSIGNMENTS 50

/* function prototypes */
int *apply_late_penalty(const int *const score, const int *const days_late,
                        const int penalty, const int num_assignments);
int *compute_values(const int *const scores, const int *const weights,
                    const int num_assignments);
double compute_standard_deviation(const double *const scores,
                                  const int num_assignments);
double compute_mean(const double *const scores, const int num_assignments);
void drop_lowest_assignment(int *scores, int *weights, int *assignment_number,
                            int *num_assignments);
void drop_assignments(int *scores, int *weights, int *assignment_number,
                      const int drops, int *num_assignments);
void copy_arr(const int *const source, int *destination, const int length);
int compute_int_sum(const int arr[], const int length);
double compute_double_sum(const double *const arr, const int length);
double compute_numeric_score(const int *const scores, const int *const weights,
                             const int *const assignment_number,
                             const int *const days_late, const int drops,
                             const int penalty, const int num_assignments);

/* main function that handles I/O */
int main(void) {
    int index = 0, weight_sum = 0, penalty, drops, num_assignments,
        target_assignment_number, *penalized_scores,
        assignment_number[MAX_NUM_ASSIGNMENTS], score[MAX_NUM_ASSIGNMENTS],
        weight[MAX_NUM_ASSIGNMENTS], days_late[MAX_NUM_ASSIGNMENTS];
    double numeric_score, penalized_score_doubles[MAX_NUM_ASSIGNMENTS];
    char stats;

    /* grabs input and sets values */
    scanf("%d%d %c%d", &penalty, &drops, &stats, &num_assignments);
    for (index = 0; index < num_assignments; index++) {
        scanf("%d,%d,%d,%d", &(assignment_number[index]), &(score[index]),
              &(weight[index]), &(days_late[index]));
    }

    /* checks weights add to 100 and terminates program if they do not */
    for (index = 0; index < num_assignments; index++) {
        weight_sum += weight[index];
    }
    if (weight_sum != 100) {
        printf("ERROR: Invalid values provided\n");
        return 0;
    }

    /* computes numeric score */
    numeric_score =
        compute_numeric_score(score, weight, assignment_number, days_late,
                              drops, penalty, num_assignments);

    /* prints standard output */
    printf("Numeric Score: %5.4f\nPoints Penalty Per Day Late: %d\nNumber of "
           "Assignments Dropped: %d\nValues Provided:\nAssignment, Score, "
           "Weight, Days Late\n",
           numeric_score, penalty, drops);
    target_assignment_number = 1;
    /* prints all assignments and their associated data */
    for (index = 0; index < num_assignments; index++) {
        if (assignment_number[index] == target_assignment_number) {
            printf("%d, %d, %d, %d \n", assignment_number[index], score[index],
                   weight[index], days_late[index]);
            index = -1;
            target_assignment_number++;
        }
    }

    /* prints statistical data if input requests so */
    if (stats == 'Y' || stats == 'y') {
        /* penalizes the scores and stores it in a new array */
        penalized_scores =
            apply_late_penalty(score, days_late, penalty, num_assignments);
        /* puts the scores in a copy array of doubles to be passed into compute
         * functions */
        for (index = 0; index < num_assignments; index++) {
            penalized_score_doubles[index] = (double)penalized_scores[index];
        }
        /* compute and print mean + standard dev */
        printf("Mean: %5.4f, Standard Deviation: %5.4f\n",
               compute_mean(penalized_score_doubles, num_assignments),
               compute_standard_deviation(penalized_score_doubles,
                                          num_assignments));
    }

    return 0;
}

/* takes in four arrays (scores, weights, assignment_number, days_late) where
 * correspoinding indexes signify the same assignmnet. takes in three ints,
 * drops, penalty, and num assignments. this function computes the numeric score
 * by dropping specified number of assignmnets of the lowest values (where value
 * = score*weight), applying a late penalty to the remaining assignments,
 * computing the sum of the values of these assignments, and then dividing that
 * by the sum of the weights of these assignmnets. this numeric score is the
 * returned as a double. this function does not modify any of its parameters. */
double compute_numeric_score(const int *const scores, const int *const weights,
                             const int *const assignment_number,
                             const int *const days_late, const int drops,
                             const int penalty, const int num_assignments) {
    int length = num_assignments, *length_ptr = &length, *penalized_scores,
        *values, scores_cpy[MAX_NUM_ASSIGNMENTS],
        weights_cpy[MAX_NUM_ASSIGNMENTS],
        assignment_number_cpy[MAX_NUM_ASSIGNMENTS];
    double numeric_score, values_sum, weights_sum;

    /* make copy arrays of scores and weights so that they do not get modified
     * during computation */
    copy_arr(scores, scores_cpy, num_assignments);
    copy_arr(weights, weights_cpy, num_assignments);
    copy_arr(assignment_number, assignment_number_cpy, num_assignments);

    /* drop the specified number of assignments */
    drop_assignments(scores_cpy, weights_cpy, assignment_number_cpy, drops,
                     length_ptr);
    /* apply the late penalty to the scores and store it in a new array */
    penalized_scores =
        apply_late_penalty(scores_cpy, days_late, penalty, length);
    /* compute the values of the assignments after they have been dropped and
     * penalized and store them in a new array */
    values = compute_values(penalized_scores, weights_cpy, length);
    /* get the sum of the values and weights */
    values_sum = ((double)(compute_int_sum(values, length)));
    weights_sum = ((double)(compute_int_sum(weights_cpy, length)));
    /* find numeric score by dividing the two */
    numeric_score = (values_sum / weights_sum);

    return numeric_score;
}

/* takes in an integer array and a length. returns the sum of each value in the
 * array up until length as an int */
int compute_int_sum(const int arr[], const int length) {
    int index, sum = 0;
    for (index = 0; index < length; index++) {
        sum += arr[index];
    }
    return sum;
}

/* takes in a double array and a length. returns the sum of each value in the
 * array up until length as an double */
double compute_double_sum(const double *const arr, const int length) {
    int index;
    double sum = 0;
    for (index = 0; index < length; index++) {
        sum += arr[index];
    }
    return sum;
}

/* takes two int arrays, source and destination. takes an integer, length.
 * copies values from source array into destination array up until the specefied
 * length. source array is NOT modified, destination array is. */
void copy_arr(const int *const source, int *destination, const int length) {
    int i;
    for (i = 0; i < length; i++) {
        destination[i] = source[i];
    }
}

/* takes in three arrays (scores, weights, and assignment_number) where
 * correspoinding indexes signify the same assignmnet. takes in an integer
 * representing the number of assignments to process as well as an integer
 * representing how many assignments must be dropped. this function then drops
 * the lowest n number of assignments (where n=drops) with the lowest value
 * (value is: score*weight) from the scores and weights arrays using the help of
 * the function drop_lowest_assignmnet(). this means the passed scores, weights,
 * and num_assignments arrays WILL BE MODIFIED. num_assignmnets is ALSO MODIFIED
 * to reflect the number of assignmnets left after the drops.*/
void drop_assignments(int *scores, int *weights, int *assignment_number,
                      const int drops, int *num_assignments) {
    int index;
    for (index = 0; index < drops; index++) {
        drop_lowest_assignment(scores, weights, assignment_number,
                               num_assignments);
    }
}

/* takes in three arrays (scores, weights, and assignment_number) where
 * correspoinding indexes signify the same assignmnet. takes in an integer
 * representing the number of assignments to process. this function then drops
 * the assignment with the lowest value (value is: score*weight) from the scores
 * and weights arrays. this means the passed scores, weights, and
 * num_assignments arrays WILL BE MODIFIED. num_assignmnet is ALSO MODIFIED to
 * reflect the number of assignmnets left after the drop. */
void drop_lowest_assignment(int *scores, int *weights, int *assignment_number,
                            int *num_assignments) {
    int index, tie_occurred = FALSE, tie_counter = 0,
               lowest_assignment_number = MAX_NUM_ASSIGNMENTS,
               index_to_drop = 0, value, lowest_value = 10001,
               index_of_lowest_value, length = *(num_assignments), *values,
               indexs_of_ties[MAX_NUM_ASSIGNMENTS] = {0};

    /* compute the values of all assignments and store it in an array */
    values = compute_values(scores, weights, length);

    /* loop thru array and look for index of lowest value while recording ties
     */
    for (index = 0; index < length; index++) {
        value = values[index];
        if (value < lowest_value) {
            index_of_lowest_value = index;
            lowest_value = value;
            /* if a new lowest value is found, we set it as the beginning of the
             * ties array and set tie_occurred to FALSE to show that no ties
             * with this value have occurred yet */
            indexs_of_ties[0] = index;
            tie_occurred = FALSE;
        } else if (value == lowest_value) {
            if (tie_occurred == FALSE) {
                /* if no other ties have occurred yet, store this tie as the
                 * second item in the ties array, set the tie_counter to reflect
                 * that the index of the next found tie should be stored as the
                 * third item in the indexes_of_ties array (index 2) and set
                 * tie_occurred to TRUE to reflect a tie has already been found.
                 */
                indexs_of_ties[1] = index;
                tie_counter = 2;
                tie_occurred = TRUE;
            } else {
                /* otherwise, store this index in indexes_of_ties array at the
                 * next available index given by tie_counter and then increment
                 * tie_counter so that the next tie may stored in the
                 * appropriate index */
                indexs_of_ties[tie_counter] = index;
                tie_counter++;
            }
        }
    }

    /* if tie_occurred is TRUE after the last loop, we know we are dealing with
     * at leat one tie. else, we can just assume the index to drop is the index
     * of the lowest value found by the loop */
    if (tie_occurred == TRUE) {
        /* if there is a tie, we must drop the score with the lowest assignment
         * number. so we must loop through the ties and check which has the
         * lowest one. */
        for (index = 0; index < tie_counter; index++) {
            if ((assignment_number[indexs_of_ties[index]]) <
                lowest_assignment_number) {
                index_to_drop = indexs_of_ties[index];
                lowest_assignment_number =
                    assignment_number[indexs_of_ties[index]];
            }
        }
    } else {
        index_to_drop = index_of_lowest_value;
    }

    /* Modify the scores, weights, and num_assignments arrays to reflect the
     * drop by starting at the index to drop and moving all values to the right
     * down one index. */
    for (index = index_to_drop; index < length - 1; index++) {
        scores[index] = scores[index + 1];
        weights[index] = weights[index + 1];
        assignment_number[index] = assignment_number[index + 1];
    }
    /* Modify the number of assigmnets to reflect the drop */
    *(num_assignments) -= 1;
}

/* takes an array of scores represented as doubles and a number of assignments
 * to process. this function then computes the mean of all the scores in the
 * array up til the number specified by num_assignmnets */
double compute_mean(const double *const scores, const int num_assignments) {
    double sum = 0, mean = 0;

    sum = compute_double_sum(scores, num_assignments);
    mean = sum / (double)num_assignments;

    return mean;
}

/* takes an array of scores represented as doubles and a number of assignments
 * to process. this function then computes the standaed deviation of all the
 * scores in the array up til the number specified by num_assignmnets */
double compute_standard_deviation(const double *const scores,
                                  const int num_assignments) {
    int index = 0, score = 0;
    double standard_deviation = 0, variance = 0,
           mean = compute_mean(scores, num_assignments),
           squares[MAX_NUM_ASSIGNMENTS];

    /* do score-mean on each score and then store the square of that in an array
     */
    for (index = 0; index < num_assignments; index++) {
        score = scores[index];
        squares[index] = (score - mean) * (score - mean);
    }
    /* find variance by computing the mean of the squares */
    variance = compute_mean(squares, num_assignments);
    standard_deviation = sqrt(variance);

    return standard_deviation;
}

/* takes two int arrays, scores and days late, where correspoinding indexes
 * signify the same assignmnet. num_assignments signifies how many assignments
 * are to be considered. penalty is the amount taken off per day late. this
 * fucntion goes through each assignment and applies a late penalty to it based
 * off how many days late it is and the given penalty per late days. the
 * resulting array of penalized scores is returned as a pointer to an integer.
 * the new array maintains the same index-assignment correspondance found in the
 * original two passed arrays. the original arrays are NOT modified. */
int *apply_late_penalty(const int *const score, const int *const days_late,
                        const int penalty, const int num_assignments) {
    int index = 0;
    static int
        penalized_scores[MAX_NUM_ASSIGNMENTS]; /* static to prevent returning
                                                  local memory */

    for (index = 0; index < num_assignments; index++) {
        penalized_scores[index] = score[index] - (days_late[index] * penalty);
        /* if the late penalty leves a score < 0 we just set the score to 0
         * instead */
        if (penalized_scores[index] < 0) {
            penalized_scores[index] = 0;
        }
    }
    return penalized_scores;
}

/* takes two int arrays, scores and weights, where correspoinding indexes
 * signify the same assignmnet. num_assignments signifies how many assignments
 * are to be considered. this fucntion goes through each assignment and computes
 * the value for each assignment (score*weight). All weights are stores in an
 * array that is then returned as a pointer to an integer. the resulting array
 * of penalized scores is returned as a pointer to an integer. the new array
 * maintains the same index-assignment correspondance found in the original two
 * passed arrays. the original arrays are NOT modified. */
int *compute_values(const int *const scores, const int *const weights,
                    const int num_assignments) {
    int index = 0;
    static int values[MAX_NUM_ASSIGNMENTS]; /* static to prevent returning local
                                               memory */

    for (index = 0; index < num_assignments; index++) {
        values[index] = scores[index] * weights[index];
    }

    return values;
}