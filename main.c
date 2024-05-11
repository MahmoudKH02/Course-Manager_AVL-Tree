
// ID: 1200275, Name: Mahmoud khatib, Sec: 4

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Course Course;
typedef struct node *Node;
typedef Node Tree;

struct Course
{
    char name[50];
    int credit;
    char code[30];
    char dep[50];
    char topics[220];
};

struct node
{
    Course Element;
    Node Right;
    Node Left;
    int Height;
};

Node MakeEmpty(Tree T)
{
    if(T != NULL)
    {
        MakeEmpty(T->Right);
        MakeEmpty(T->Left);
        free(T);
    }
    return NULL;
}

Node Find(char *x, Tree T)
{
    if(T == NULL)
        return NULL;

    if(stricmp(x, T->Element.code) < 0)
        Find(x, T->Left);
    else if(stricmp(x, T->Element.code) > 0)
        Find(x, T->Right);
    else
        return T;
}

Node FindMin(Tree T)
{
    if(T == NULL)
        return NULL;
    else if(T->Left == NULL)
        return T;
    else
        FindMin(T->Left);
}

Node FindMax(Tree T)
{
    if(T == NULL)
        return NULL;
    else if(T->Right == NULL)
        return T;
    else
        FindMax(T->Right);
}

int Height(Node p)
{
    if(p == NULL)
        return -1;
    else
        return p->Height;
}

int getBalance(Tree T)
{
    if(T == NULL)
        return 0;
    else
        return Height(T->Left) - Height(T->Right);
}

int Max(int Lhs, int Rhs)
{
    return Lhs > Rhs ? Lhs : Rhs;
}

/* left of left */
Node SingleRotateWithLeft(Node P2)
{
    Node P1;

    P1 = P2->Left;
    P2->Left = P1->Right;
    P1->Right = P2;

    P2->Height = Max(Height(P2->Left), Height(P2->Right)) + 1;
    P1->Height = Max(Height(P1->Left), P2->Height) + 1;

    return P1;
}

/* right of right */
Node SingleRotateWithRight(Node P1)
{
    Node P2;

    P2 = P1->Right;
    P1->Right = P2->Left;
    P2->Left = P1;

    P1->Height = Max(Height(P1->Left), Height(P1->Right)) + 1;
    P2->Height = Max(Height(P2->Right), P1->Height) + 1;

    return P2;
}

/* right of left */
Node DoubleRotateWithLeft(Node P1)
{
    P1->Left = SingleRotateWithRight(P1->Left);

    return SingleRotateWithLeft(P1);
}

/* left of right */
Node DoubleRotateWithRight(Node P1)
{
    P1->Right = SingleRotateWithLeft(P1->Right);

    return SingleRotateWithRight(P1);
}

/* will balance a certain Node */
Node BalanceNode(Node T)
{
    int balance = Height(T->Left) - Height(T->Right);

    if(balance > 1 && getBalance(T->Left) >= 0)
        T = SingleRotateWithLeft(T);

    if(balance > 1 && getBalance(T->Left) < 0)
        T = DoubleRotateWithLeft(T);

    if(balance < -1 && getBalance(T->Right) <= 0)
        T = SingleRotateWithRight(T);

    if(balance < -1 && getBalance(T->Right) > 0)
        T = DoubleRotateWithRight(T);

    return T;
}

/* re-balance a tree */
Tree RebalanceTree(Tree T)
{
    if(T == NULL)
        return T;

    T->Left = RebalanceTree(T->Left);
    T->Right = RebalanceTree(T->Right);

    if(T->Left != NULL || T->Right != NULL)
        T = BalanceNode(T);

    return T;
}

Node Insert(Course x, Tree T)
{
    if(T == NULL)
    {
        T = (Node)malloc(sizeof(struct node));

        if(T == NULL)
            printf("Out of Space\n");
        else
        {
            T->Element = x;
            T->Height = 0;
            T->Right = T->Left = NULL;
        }
    }
    else if(stricmp(x.code, T->Element.code) < 0)
    {
        T->Left = Insert(x, T->Left);

        if(Height(T->Left) - Height(T->Right) == 2)
        {
            if(stricmp(x.code, T->Left->Element.code) < 0)
                T = SingleRotateWithLeft(T);
            else
                T = DoubleRotateWithLeft(T);
        }
    }
    else if(stricmp(x.code, T->Element.code) > 0)
    {
        T->Right = Insert(x, T->Right);

        if(Height(T->Right) - Height(T->Left) == 2)
        {
            if(stricmp(x.code, T->Right->Element.code) > 0)
                T = SingleRotateWithRight(T);
            else
                T = DoubleRotateWithRight(T);
        }
    }
    T->Height = Max(Height(T->Left), Height(T->Right)) + 1;

    return T;
}

/* x is the code for the course that is to be deleted */
Node Delete(char *x, Tree T)
{
    if(T == NULL)
        return NULL;

    if(stricmp(x, T->Element.code) < 0)
        T->Left = Delete(x, T->Left);
    else if(stricmp(x, T->Element.code) > 0)
        T->Right = Delete(x, T->Right);
    else
    {
        if(T->Right == NULL || T->Left == NULL)
        {
            Node temp = T->Left ? T->Left : T->Right;

            if(temp == NULL)
            {
                temp = T;
                T = NULL;
            }
            else
                *T = *temp;

            free(temp);
        }
        else
        {
            Node temp = FindMin(T->Right);

            T->Element = temp->Element;
            T->Right = Delete(temp->Element.code, T->Right);
        }
    }
    if(T == NULL)
        return T;

    T->Height = Max(Height(T->Right), Height(T->Left)) + 1;

    T = BalanceNode(T);

    return T;
}

void displayCourseInfo(Course c)
{
    printf("Course: %s\n", c.name);
    printf("Credit Hours: %d\n", c.credit);
    printf("Code: %s\t", c.code);
    printf("Department: %s\n", c.dep);
    printf("Topics: %s\n", c.topics);
    printf("-----------------------------\n");
}

int getSize(Tree T)
{
    if(T == NULL)
        return 0;

    return 1 + getSize(T->Left) + getSize(T->Right);
}

/**
 * **********************************
 * end of Tree Library functions
 * **********************************
**/


Tree readFromFile(Tree tree)
{
    tree = MakeEmpty(tree);
    FILE *file;
    char buffer[1024];

    file = fopen("courses.txt", "r");

    while(fgets(buffer, 1024, file) != NULL)
    {
        Course c;

        // will skip any empty line
        if(strlen(buffer) > 1)
        {
            strcpy(c.name, strtok(buffer, ":"));
            c.credit = atoi(strtok('\0', "#"));
            strcpy(c.code, strtok('\0', "#"));
            strcpy(c.dep, strtok('\0', "/"));
            strcpy(c.topics, strtok('\0', "\n"));

            tree = Insert(c, tree);
        }
    }
    fclose(file);

    return tree;
}

Tree insertCourseFromUser(Tree tree)
{
    Course c;

    printf("Please Enter course Name: ");
    scanf("\n%[^\n]", c.name);

    printf("\nPlease Enter course credit Hours: ");
    scanf("%d", &c.credit);

    printf("\nPlease Enter course code: ");
    scanf("\n%[^\n]", c.code);
    strcpy(c.code, strupr(c.code));

    printf("\nPlease Enter course Department: ");
    scanf("\n%[^\n]", c.dep);

    printf("\nPlease Enter course topics: ");
    scanf("\n%[^\n]", c.topics);

    tree = Insert(c, tree);

    return tree;
}

Tree findAndUpdate(Tree tree)
{
    Node p;
    char temp[50];
    int n;

    printf("Enter the code for the course you want to find\n");
    scanf("%s", temp);

    p = Find(temp, tree);

    if(p == NULL)
    {
        printf("The item you are looking for DOES NOT EXIST!!!\nPlease enter an existing Code.\n\n");
        return tree;
    }
    displayCourseInfo(p->Element);

    printf("Do you want to update this course info?\n1-Yes or 2-No\n");
    scanf("%d", &n);

    if(n == 1)
    {
        Course updatedCourse = p->Element;

        do
        {
            printf("what data you wish to update:\n");
            printf("1-Name.\t2-Credit.\t3-Code.\t4-Department.\t5-Topics.\t6-done.\n");
            scanf("%d", &n);

            switch(n)
            {
            case 1:
                printf("Please enter the new Name: ");
                scanf("\n%[^\n]", updatedCourse.name);
                break;
            case 2:
                printf("\nPlease enter the new credit Hours: ");
                scanf("%d", &updatedCourse.credit);
                break;
            case 3:
                printf("\nPlease enter the new Course Code: ");
                scanf("\n%[^\n]", updatedCourse.code);
                break;
            case 4:
                printf("\nPlease enter the new Department: ");
                scanf("\n%[^\n]", updatedCourse.dep);
                break;
            case 5:
                printf("\nPlease enter the new Topics: ");
                scanf("\n%[^\n]", updatedCourse.topics);
                break;
            case 6:
                printf("\nChanges has been Saved...\n\n");
                break;
            }
        }while(n != 6);

        tree = Delete(p->Element.code, tree);
        tree = Insert(updatedCourse, tree);
    }
    return tree;
}

Tree delSpecificLetter(char letter, Tree tree)
{
    if(tree == NULL)
        return NULL;

    tree->Left = delSpecificLetter(letter, tree->Left);
    tree->Right = delSpecificLetter(letter, tree->Right);

    if(tree->Element.name[0] == (letter))
        tree = Delete(tree->Element.code, tree);

    // the deletion of the node will not balance the node because the delete function will not have
    // access to the parent to check it's balance.
    tree = RebalanceTree(tree);

    return tree;
}

Tree delAllDepCourses(char *depart, Tree tree)
{
    if(tree == NULL)
        return NULL;

    tree->Left = delAllDepCourses(depart, tree->Left);
    tree->Right = delAllDepCourses(depart, tree->Right);

    if(stricmp(tree->Element.dep, depart) == 0)
        tree = Delete(tree->Element.code, tree);

    tree = RebalanceTree(tree);

    return tree;
}

void swap(Course *a, Course *b)
{
    Course temp = *a;
    *a = *b;
    *b = temp;
}

/* used in quick sort */
int partition(Course *array, int low, int high)
{
    Course pivot = array[high];

    int i = low - 1;

    for(int j = low; j < high; j++)
    {
        if(stricmp(array[j].name, pivot.name) <= 0)
        {
            i++;
            swap(&array[i], &array[j]);
        }
    }
    swap(&array[i + 1], &array[high]);

    return (i + 1);
}

/* uses quick sort */
void sort(Course *array, int low, int high)
{
    if(low < high)
    {
        int p = partition(array, low, high);

        sort(array, low, p - 1);
        sort(array, p + 1, high);
    }
}

/* copies the tree to an array.
takes a pointer to the index of the array starting from last index
all the way to the first one */
void copyToArray(Tree tree, Course *array, int *index)
{
    if(tree != NULL)
    {
        array[*index - 1] = tree->Element;
        *index -= 1;
        copyToArray(tree->Left, array, index);
        copyToArray(tree->Right, array, index);
    }
}

/* prints the courses sorted by the course name
if dep is NULL then all the courses are printed. */
void lexiListCourses(Tree tree, char *dep)
{
    int size = getSize(tree);
    Course list[size];

    int temp = size; // since the size is used again in the function so we send the address for the temp value.
    copyToArray(tree, list, &temp);
    sort(list, 0, size - 1);

    for(int i = 0; i < size; i++)
    {
        if(dep != NULL && stricmp(list[i].dep, dep) != 0)
            continue;

        displayCourseInfo(list[i]);
    }
}

void saveToFile(Tree tree, FILE *file)
{
    if(tree != NULL)
    {
        saveToFile(tree->Left, file);

        fprintf(file, "%s:", tree->Element.name);
        fprintf(file, "%d#", tree->Element.credit);
        fprintf(file, "%s#", tree->Element.code);
        fprintf(file, "%s/", tree->Element.dep);
        fprintf(file, "%s\n", tree->Element.topics);

        saveToFile(tree->Right, file);
    }
}

int main()
{
    Tree tree = NULL;
    tree = MakeEmpty(tree);
    int menu;
    short dataIsLoaded = 0; // flag to determine if the data has been loaded from the file.
    FILE *output;

    printf("Starting program...\n\n");

    do
    {
        char scanner[50]; // used to take some input.

        printf("Please select an item from the following menu:\n");
        printf("1.Read the file courses.txt file and create the tree.\n");
        printf("2.Insert a new course from the user with all its associated data.\n");
        printf("3.Find a course and support updating of its information.\n");
        printf("4.List courses in lexicographic order with their associated information.\n");
        printf("5.List all topics associated with a given course.\n");
        printf("6.List all courses in lexicographic order that belong to the same department\n");
        printf("7.Delete a course.\n");
        printf("8.Delete all courses that start with a specific letter.\n");
        printf("9.Delete all courses belong to a given department.\n");
        printf("10.Save all words in file and exit the program.\n");
        scanf("%d", &menu);

        if(!dataIsLoaded && menu != 10 && menu != 1)
        {
            printf("\nPlease load the data from file first then try again\n\n");
            continue;
        }

        switch(menu)
        {
        case 1:
            tree = readFromFile(tree);
            printf("Data has been successfully Loaded from File.\n\n");
            dataIsLoaded = 1;
            break;

        case 2:
            tree = insertCourseFromUser(tree);
            break;

        case 3:
            tree = findAndUpdate(tree);
            break;

        case 4:
            lexiListCourses(tree, NULL); // NULL sent to determine that all courses need to be printed.
            break;

        case 5:
            printf("Enter course code: ");
            scanf("%s", scanner);
            printf("\nTopics: ");
            printf("%s\n", Find(scanner, tree)->Element.topics);
            break;

        case 6:
            printf("Enter a certain department: ");
            scanf("\n%[^\n]", scanner);
            lexiListCourses(tree, scanner);
            break;

        case 7:
            printf("\nEnter the code for the course you wish to delete: ");
            scanf("%s", scanner);
            tree = Delete(scanner, tree);
            break;

        case 8:
            printf("\nEnter a letter to all courses that start with it: ");
            scanf("\n%c", scanner);
            tree = delSpecificLetter(scanner[0], tree);
            break;

        case 9:
            printf("\nEnter the department you wish to delete all it's courses: ");
            scanf("\n%[^\n]", scanner);
            tree = delAllDepCourses(scanner, tree);
            break;

        case 10:
            printf("\nSaving data...\n");
            output = fopen("offered_courses.txt", "w");
            saveToFile(tree, output);
            fclose(output);
            break;

        default:
            printf("\nPlease enter a valid option and try again\n\n");
            break;
        }
    }while(menu != 10);

    tree = MakeEmpty(tree);

    return 0;
}
