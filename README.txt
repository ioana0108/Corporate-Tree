
sort()
	- sorteaza ierarhia lexicografic



preorder_traversal()
	- afiseaza ierarhia


hire()
	- creeaza ierarhia
	- aloca memorie pentru primul unicul nod din ierarhie
	- fiind singurul, i se va atribui numele anagajatului primit drept parametru
	- returneaza adresa nodului
	- gaseste nodul deja existent (parintele) de care trebuie legat noul nod
	- incrementeaza nr de angajati ai managerului de care va fi legat noul angajat
	- aloca memorie pt acest nou angajat
	- atribuie nodului numele dat ca parametru
	- parintele noului nod va fi tree (nodul gasit anterior)
	- il adauga in vectorul de angajati
	- parcurge arborele pana gaseste nodul cu numele dat ca parametru "manager_name"


fire()
	- verifica daca a gasit bietul angajat
	- calculeaza pozitia angajatului de concediat in team-ul manager-ului sau si o 
retine in poz, pentru a putea fi ulterior eliminat din team
	- il elimina din team-ul din care facea parte
	- scade nr de angajati ai manager ului angajatului concediat
	- parcurge angajatii nodului de concediat
	- alipeste fiecare angajat la managerul angajatului concediat si incrementeaza 
nr de angajati ai sai la fiecare pas
	- ii integreaza in noua echipa
	- parcurge arborele pana gaseste nodul cu numele dat ca parametru "employee_name"
	- sorteaza ierarhia cu functia sort()


promote()
	- apeleaza functia fire() pentru a sterge angajatul de pe pozitia in care a fost
initial in ierarhie
	- apeleaza functia hire() pentru a-l reangaja in pozitia dorita
	- returneaza ierarhia


move_employee()
	- daca a gasit angajatul de mutat, il sterge folosind functia fire() si il
reangajaza cu hire() pe pozitia dorita



find_employee() - functie extra
	- returneaza adresa angajatului cu numele primit drept parametru



move_team()
	- - calculeaza pozitia angajatului din varful echipei in team-ul manager-ului sau si o 
retine in poz, pentru a putea fi ulterior eliminat din team
	- il elimina din team-ul din care facea parte
	- il adauga in echipa altui manager



fire_team()
	- cat timp un angajat mai are angajati, apeleaza functia fire() pentru a-i concedia
pe rand
	- trece recursiv la urmatorul angajat din echipa de concediat
	- returneaza ierarhia modificata



find_employees() - functie extra
	- creeaza un vector ce contine toti subalternii unui angajat primit drept parametru




get_employees_by_manager()
	- apeleaza functia find_employees 
	- sorteaza vectorul obtinut 
	- afiseaza in fisier rezultatul



tree_queue()
	- transpune ierarhia intr-o lista de tip coada
	- retine in parametrii functiei primul nod si ultimul nod de pe nivelul cerut


get_employees_by_level()
	- apeleaza functia tree_queue() daca primeste un nivel valid, adica mai mare decat 0
	- pentru 0, afiseaza radacina
	- sorteaza coada intre valorile first_level_node si last_level_node
	- afiseaza bucata de coada dintre first_level_node si last_level_node sortata,
reprezentand nivelul cerut























