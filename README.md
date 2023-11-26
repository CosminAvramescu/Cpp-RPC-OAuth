# Cpp-RPC-Oauth
# 344C3_Avramescu_Cosmin_Alexandru

---Aspecte generale

    Am facut tema in C++. Dupa rularea rpcgen, fisierele pe care le-am modificat sunt: tema_server.cpp, tema_client.cpp, tema_svc.cpp, tema.h si Makefile.

--- Makefile
    Am schimbat ce era cu .c in .cpp pentru a putea rula tema cu fisierele C++. 

--- tema.h
    Aici doar am adaugat alte biblioteci (map, vector, string, etc) si am definit variabilele globale
cu extern. Am folosit aceste variabile globale in tema_svc.cpp (pentru initializare, la citirea fisierelor de intrare) si in tema_server.cpp (pentru actualizarea valorilor, lucrul cu baza de date).
    Am scris in dreptul fiecarei structuri la ce am folosit-o. 

--- tema_svc.cpp
    Aici doar import variabilele globale din tema.h si deschid fisierele date ca parametru la rularea 
serverului (userIds, resources si approvals). Aici citesc efectiv userIds si resources, iar approvals doar este deschis si citesc cate o linie din el in server la fiecare Request facut. Deasemenea, aici initializez variabila globala valability, care este primita tot ca argument. 

--- tema_server.cpp 
    Aici am implementat functiile serverului. Pe langa cele pe care le-am identificat din cerinta
temei (request_authorization_1, request_access_token_1, validate_delegated_action_1,approve_request_token_1_svc), am mai implementat eu check_valability_1. Am avut nevoie de aceasta deoarece trebuia sa stiu in client daca este nevoie sa se faca refresh-ul automat al tokenului (daca valabilitatea tokenului a ajuns la 0). Din moment ce in client nu am acces la baza de date users, a fost nevoie sa obtin informatia de valabilitate de la server (deci de asta am implementat aceasta functie).

    --- request_authorization_1
         Aici primesc userId si caut in baza de date users pana cand gasesc userul care ma intereseaza.
    Daca nu s-a gasit userul, se intoarce in result USER_NOT_FOUND. Apoi generez un token (requestToken) pe care il pun in result. Dupa ce a fost generat requestToken, se citeste o linie din fisierul
    approvals pentru a vedea ce permisiuni are userul curent. Se salveaza la cheia requestToken in map-ul approvals vectorul de stucturi cu resursa si permisiunile citite. Apoi se returneaza result.

    --- request_access_token_1
        Se primesc userId, requestToken si 2 variabile bool. Am avut nevoie de variabila bool 
    refreshToken pentru a sti daca trebuie generat sau nu si refreshToken. Am avut nevoie de variabila
    bool beginRefresh pentru a sti daca incepe procedura de reactualizare a tokenilor sau nu. Se cauta dupa id userul in baza de date, apoi se verifica daca are tokenul validat, in caz contrar se intoarce REQUEST_DENIED. Daca a fost validat tokenul, se genereaza accessToken. Daca refreshToken e true, se 
    genereaza si refreshToken. Se actualizeaza in baza de date users valorile accessToken si refreshToken.

    --- validate_delegated_action_1
        Se primesc accessToken, operatia si resursa. Se cauta dupa accessToken userul in baza de date.
    Daca tokenul nu a fost validat, se intoarce OPERATION_NOT_PERMITTED. Altfel, se verifica valabilitatea tokenului. Daca e 0, se intoarce TOKEN_EXPIRED, altfel se verifica daca userul are permisiuni pentru a realiza operatia dorita. Daca nu exista resursa, se intoarce RESOURCE_NOT_FOUND, 
    altfel se cauta in vectorul de permisiuni din approvals daca exista permisiuni pentru resursa solicitata. Se extrag permisiunile din approvals si se compara cu prima litera din operation (de exemplu daca avem permisiuni RMD si operatie READ, o sa verificam daca prima litera din READ - R se regaseste in RMD). Doar la execute verificam a 2-a litera (X). Daca exista permisiuni, se intoarce
    PERMISSION_GRANTED, altfel se intoarce OPERATION_NOT_PERMITTED. Daca nu a fost gasit accessToken-ul userului in baza de date, se intoarce PERMISSION_DENIED. 

    --- approve_request_token_1_svc
        Se parcurge map-ul approvals pentru a vedea daca la cheia requestToken se afla permisiunile
    *-. Daca se gasesc aceste permisiuni, se invalideaza tokenul prin variabila validity din baza de date users. Apoi se intoarce requestToken.

--- tema_client.cpp
    Am folosit 3 variabile globale, userData (map intre userId si accessToken - pentru a extrage accessToken si a il trimite la validate_delegated_action_1), userRefresh (map intre userId si refreshToken - pentru a trimite refreshToken pe post de requestToken la request_access_token_1 pentru reinoirea automata) si automatedRefresh (map intre userId si bool - pentru a sti daca se face sau nu reactualizarea tokenilor).

    --- authz_and_access
        Am facut aceasta functie pentru a elimina codul duplicat, deoarece in ambele cazuri (REQUEST 0 si
    REQUEST 1) se apeleaza cele 3 functii de authorize, approve si access din server. Aici se construiesc structurile pentru fiecare apel de functie si se dau ca argumente mai departe serverului. Apoi se actualizeaza map-urile userData, userRefresh si automatedRefresh cu noile valori obtinute si se afiseaza valorile tokenilor obtinuti. 

    --- oauth_1
        Se citeste linie cu linie din client.in si se executa operatiile. Daca avem REQUEST 0 sau 1, se
    apeleaza authz_and_access. Apoi se verifica ce valabilitate mai are jetonul userului curent, iar daca
    valabilitatea este 0 si userul are reinoire automata, atunci se apeleaza metoda request_access_token_1 si se actualizeaza din nou valorile din map-urile userData, userRefresh. Daca operatia nu este de tip Request (adica este de READ, DELETE etc), atunci se construieste structura si se apeleaza metoda validate_delegated_action_1.