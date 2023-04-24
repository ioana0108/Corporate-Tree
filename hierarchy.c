#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hierarchy.h"

/* Adauga un angajat nou in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat
 * manager_name: numele sefului noului angajat OK
 *
 * return: intoarce ierarhia modificata. Daca tree si manager_name sunt NULL, // haide mai o putem schimba daca chiar vrei,  
           atunci employee_name e primul om din ierarhie.
 */
/* typedef struct TreeNode {
    char *name;                 // numele angajatului
    int direct_employees_no;    // numarul de copii ai nodului curent
    struct TreeNode **team;     // copiii nodului curent
    struct TreeNode *manager;   // parintele nodului curent
} TreeNode, *Tree;
*/

int contains(Tree tree, char *string)
{
    int i, s = 0;

    if (strcmp(string, tree->name) == 0)
    {              
        return 1; 
    }
    else
    {
        for (i = 0; i < tree->direct_employees_no; i++)
        {
            s += contains(tree->team[i], string);
        }
        if (s > 0)
        {
            return 1; 
        }
    }

    return 0; 
}

void sort(Tree tree)
{
    int i, j;

    Tree aux;
    if (tree == NULL)
    {
        return; 
    }

    for (i = 0; i < tree->direct_employees_no; i++)
    {
        for (j = i + 1; j < tree->direct_employees_no; j++)
        {
            if (strcmp(tree->team[i]->name, tree->team[j]->name) > 0)
            {
                aux = tree->team[i];
                tree->team[i] = tree->team[j];
                tree->team[j] = aux;
            }
        }
    }
}

void preorder_traversal(FILE *f, Tree tree)
{
    int i = 0;

    if (tree == NULL) 
    {
        return;
    }

    int j, nr_nodes = 1, contor = 0;

    if (tree->manager == NULL) 
    {
        fprintf(f, "%s ", tree->name);
    }
    else 
    {
        fprintf(f, "%s-%s ", tree->name, tree->manager->name);
    }

    for (i = 0; i < tree->direct_employees_no; i++) 
    {
        if (tree->team[i] != NULL)
        {
            preorder_traversal(f, tree->team[i]);
        }
    }

    if (tree->manager == NULL)
    {
        fprintf(f, "\n");
    }
}

Tree hire(Tree tree, char *employee_name, char *manager_name)
{

    if (tree == NULL) 
    {
        Tree tree = calloc(1, sizeof(TreeNode)); 

        tree->name = strdup(employee_name); 

        tree->manager = NULL; 

        return tree; 
    }

    if (strcmp(tree->name, manager_name) == 0) 
    {

        if (tree->team == NULL) 
        {
            tree->team = calloc(100, sizeof(Tree));
        }

        tree->direct_employees_no++; 

        Tree newnode = calloc(1, sizeof(TreeNode)); 

        newnode->name = strdup(employee_name); 

        newnode->manager = tree; 

        tree->team[tree->direct_employees_no - 1] = newnode; 

        sort(tree);
    }
    else
    {
        int i;
        for (i = 0; i < tree->direct_employees_no; i++) 
        {
            tree->team[i] = hire(tree->team[i], employee_name, manager_name);
        }
    }

    return tree;
}

/* Sterge un angajat din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului concediat
 *
 * return: intoarce ierarhia modificata.
 */
Tree fire(Tree tree, char *employee_name) 
{                                         

    if (tree == NULL) 
    {
        return NULL;
    }
    if (strcmp(tree->name, employee_name) == 0) 
    {

        if (tree->manager == NULL)
        {
            return tree;
        }

        int i, poz = 0;
        for (i = 0; i < tree->manager->direct_employees_no; i++)
        {
            if (strcmp(tree->manager->team[i]->name, tree->name) == 0) 
            {
                poz = i; 
                i = tree->manager->direct_employees_no;
            }
        }

        for (i = poz; i < tree->manager->direct_employees_no - 1; i++) 
        {
            tree->manager->team[i] = tree->manager->team[i + 1]; 
        }                                                        
        tree->manager->direct_employees_no--;                    

        int nr = tree->manager->direct_employees_no;

        for (i = 0; i < tree->direct_employees_no; i++) 
        {
            tree->team[i]->manager = tree->manager; 

            tree->manager->direct_employees_no++;

            tree->manager->team[tree->manager->direct_employees_no - 1] = tree->team[i];

        } 

        Tree man = tree->manager; 
        
        if (tree->team != NULL)
        {
            free(tree->team);
        }

        free(tree->name);
        free(tree);            
        return man->team[poz]; 
        
    }
    else
    {
        int i;
        for (i = 0; i < tree->direct_employees_no; i++) 
        {
            
            //if (contains(tree, employee_name) == 1)
            {
                tree->team[i] = fire(tree->team[i], employee_name); 
            }                                                                                 
        }
        
    }            
    sort(tree);  
    return tree; 
}

Tree find_manager(Tree tree, char *employee_name)
{
    int i;
    Tree manager = NULL;

    if (tree == NULL)
    {
        return tree;
    }

    if (strcmp(tree->name, employee_name) == 0) 
    {
        manager = tree->manager; 
        return manager;
    }
    else
    {
        for (i = 0; i < tree->direct_employees_no && manager == NULL; i++)
        {
            
            manager = find_manager(tree->team[i], employee_name); 

            
        }
    }
    return manager;
}

/* Promoveaza un angajat in ierarhie. Verifica faptul ca angajatul e cel putin 
 * pe nivelul 2 pentru a putea efectua operatia.
 * 
 * tree: ierarhia existenta
 * employee_name: numele noului angajat  
 *
 * return: intoarce ierarhia modificata.
 */
Tree promote(Tree tree, char *employee_name)
{

    int i, poz;
    Tree manager = NULL;

    manager = find_manager(tree, employee_name); 

    if (manager != NULL && manager->manager != NULL)
    {
        tree = fire(tree, employee_name); // aici nu trebuia tree = fire()? ba da
        tree = hire(tree, employee_name, manager->manager->name);
    }
    
    return tree; 

} // nu  pot sa inteleg, ce treaba au functiile hire si fire, de ce nu le mai ia ca finnd corecte acum da bravo ai schimbat printf la loc?nuuuu dar ia seg sa vedem unde

/* Muta un angajat in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */
Tree move_employee(Tree tree, char *employee_name, char *new_manager_name)
{
    Tree manager = NULL;

    manager = find_manager(tree, employee_name);

    if (tree == NULL)
    {
        return NULL;
    }
    else if (manager != NULL) 
    {
        if (strcmp(manager->name, new_manager_name) != 0) 
        {
            tree = fire(tree, employee_name);
            tree = hire(tree, employee_name, new_manager_name); 
        }
    }

    return tree;
}

Tree find_employee(Tree tree, char *employee_name)
{
    int i;
    Tree employee = NULL;

    if (strcmp(tree->name, employee_name) == 0)
    {
        employee = tree;

        return employee;
    }
    else
    {
        for (i = 0; i < tree->direct_employees_no && employee == NULL; i++)
        {

            employee = find_employee(tree->team[i], employee_name); 
        }
    }

    return employee;
}

/* Muta o echipa in ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei mutate
 * new_manager_name: numele noului sef al angajatului
 *
 * return: intoarce ierarhia modificata.
 */

Tree move_team(Tree tree, char *employee_name, char *new_manager_name)
{

    int i, gasit = 0;
    Tree new_manager = NULL;
    Tree employee = NULL;

    if (tree == NULL)
    {
        return NULL;
    }

    employee = find_employee(tree, employee_name); 

    new_manager = find_employee(tree, new_manager_name); 

    int poz;

    if (new_manager == NULL || employee == NULL)
    {
        return tree;
    }
    if (new_manager == employee->manager)
    {
        return tree;
    }

    for (i = 0; i < employee->manager->direct_employees_no; i++)
    {
        if (strcmp(employee->manager->team[i]->name, employee_name) == 0)
        {
            poz = i;
            i = employee->manager->direct_employees_no; 
        }
    }

    for (i = poz; i < employee->manager->direct_employees_no; i++)
    {
        employee->manager->team[i] = employee->manager->team[i + 1];
    }
    employee->manager->direct_employees_no--; 

    new_manager->direct_employees_no++;

    if (new_manager->team == NULL)
    {
        new_manager->team = calloc(100, sizeof(Tree));
    }

    new_manager->team[new_manager->direct_employees_no - 1] = employee;

    employee->manager = new_manager;

    sort(new_manager);

    return tree; 
}

/* Concediaza o echipa din ierarhie.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei concediate
 *
 * return: intoarce ierarhia modificata.
 */

Tree fire_team(Tree tree, char *employee_name)
{

    Tree employee;

    employee = find_employee(tree, employee_name);

    if (employee != NULL)
    {
        while (employee->direct_employees_no != 0)
        {
            tree = fire(tree, employee->team[0]->name);
        }

        tree = fire(tree, employee_name);
    }

    return tree;
}

/* Afiseaza toti angajatii sub conducerea unui angajat.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * employee_name: numele angajatului din varful echipei
 */

void find_employees(Tree tree, Tree employee, Tree **employees, int *nr)
{
    if (*employees == NULL)
    {
        *employees = calloc(100, sizeof(Tree)); 
    }

    int i;
    if (employee != NULL)
    {
        (*employees)[*nr] = employee;
        (*nr)++; 
        for (i = 0; i < employee->direct_employees_no; i++)
        { 
            (*employees)[*nr] = employee->team[i]; 
            find_employees(tree, employee->team[i], employees, nr); 
        }                                                           
    }                                                               
                                                                
}

void get_employees_by_manager(FILE *f, Tree tree, char *employee_name)
{

    int i, j, nr = 0;
    Tree *employees = NULL;
    Tree aux = NULL, employee = NULL;

    employee = find_employee(tree, employee_name);

    find_employees(tree, employee, &employees, &nr);

    for (i = 0; i < nr; i++)
    {
        for (j = i + 1; j < nr; j++)
        {
            if (strcmp(employees[i]->name, employees[j]->name) > 0)
            {
                aux = employees[i];
                employees[i] = employees[j];
                employees[j] = aux;
            }
        }
    }

    for (i = 0; i < nr; i++)
    {
        fprintf(f, "%s ", employees[i]->name);
    }
    fprintf(f, "\n");
    free(employees);
}

/* Afiseaza toti angajatii de pe un nivel din ierarhie.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 * level: nivelul din ierarhie
 */
void tree_queue(Tree tree, Tree **queue, int *first_level_node, int *last_level_node, int level, int *nr)
{
    (*queue)[*nr] = tree; 
    (*nr)++;
    int nivel = 0, last_increment = 0;
    for (int i = 0; i < *nr; i++)
    {
        if (i == 1)
        {
            nivel++;
            if (level == nivel)
            {
                *first_level_node = i;
            }
        }
        else if (i > tree->direct_employees_no && i >= last_increment + (*queue)[i]->manager->manager->direct_employees_no && (*queue)[i]->manager->team[0] == (*queue)[i]) // nu e buna cond asta lipseste faptul ca s-a mai facut o data ++ si la 2 si se mai face si la 4 stai
        {
            nivel++; 
            last_increment = i;
            if (nivel == level)
            {
                *first_level_node = i;
            } 
            if (nivel == level + 1)
            {
                *last_level_node = i - 1;
            }
        }

        for (int j = 0; j < (*queue)[i]->direct_employees_no; j++)
        {
            (*queue)[*nr] = (*queue)[i]->team[j]; 
            (*nr)++;
        } 
    }
}

void get_employees_by_level(FILE *f, Tree tree, int level)
{
    int nr = 0;
    Tree *queue = calloc(50, sizeof(Tree));
    int first_level_node = -1, last_level_node = -1;
    if (level < 0)
    {
        fprintf(f,"\n");
    }
    else if (level == 0)
    {
        fprintf(f,"%s \n", tree->name);
    }
    else
    {
        tree_queue(tree, &queue, &first_level_node, &last_level_node, level, &nr); 
        if (last_level_node < first_level_node)
        {
            last_level_node = nr - 1;
        }
        if(last_level_node == -1 || first_level_node == -1)
        {
            fprintf(f,"\n"); 
        }
        else
        {
            int i, j;
            Tree aux = NULL;

            for(i = first_level_node; i <= last_level_node; i++)
            {
                for(j = i + 1; j <= last_level_node; j++)
                {
                    if(strcmp(queue[i]->name, queue[j]->name) > 0)
                    {
                        aux = queue[i];
                        queue[i] = queue[j];
                        queue[j] = aux; 
                    }
                }
            }
            for( i = first_level_node; i <= last_level_node ; i++)
            {
                fprintf(f,"%s ", queue[i]->name);
            }
            fprintf(f,"\n");
        }
        
       
    } 
    free(queue);
    
} 

/* Afiseaza angajatul cu cei mai multi oameni din echipa.
 * 
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

void find_max_no_employees(Tree tree, int *maxim)
{
    int i;

    for (i = 0; i < tree->direct_employees_no; i++)
    {
        if (tree->direct_employees_no > *maxim && tree != NULL)
        {
            *maxim = tree->direct_employees_no;
        }

        find_max_no_employees(tree->team[i], maxim);
    }
}

void get_best_manager(FILE *f, Tree tree)
{
    Tree *employees = NULL, *best_managers = NULL, aux = NULL;
    int counter1 = 0, counter2 = 0, maxim = 0, i, j;
    best_managers = calloc(50, sizeof(Tree));
    find_employees(tree, tree, &employees, &counter1);

    find_max_no_employees(tree, &maxim);

    for (i = 0; i < counter1; i++)
    {
        if (employees[i]->direct_employees_no == maxim)
        {
            best_managers[counter2] = employees[i];
            counter2++;
        }
    }

    for (i = 0; i < counter2; i++)
    {
        for (j = i + 1; j < counter2; j++)
        {
            if (strcmp(best_managers[i]->name, best_managers[j]->name) > 0)
            {
                aux = best_managers[i];
                best_managers[i] = best_managers[j];
                best_managers[j] = aux;
            }
        }
    }

    for (i = 0; i < counter2; i++)
    {
        fprintf(f, "%s ", best_managers[i]->name);
    }
    fprintf(f, "\n");
    free(employees);
    free(best_managers);
}

/* Reorganizarea ierarhiei cu un alt angajat in varful ierarhiei.
 * 
 * tree: ierarhia existenta
 * employee_name: numele angajatului care trece in varful ierarhiei
 */
Tree reorganize(Tree tree, char *employee_name)
{
    return tree;
}

/* Parcurge ierarhia conform parcurgerii preordine.
 *
 * f: fisierul in care se va face afisarea
 * tree: ierarhia existenta
 */

/* Elibereaza memoria alocata nodurilor din arbore
 *
 * tree: ierarhia existenta
 */
void destroy_tree(Tree tree) 
{
    if(tree == NULL)
    {
        return;
    }
    int i;
    for(i = 0; i < tree->direct_employees_no; i++)
    {
        destroy_tree(tree->team[i]);
    }
    free(tree->name);
    if(tree->team != NULL)
    {
        free(tree->team);
    }
    free(tree);
}