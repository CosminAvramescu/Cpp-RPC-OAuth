/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "app.h"
#include "token.h"

map<string, vector<struct resourcesPerm>> approvals;

char *generate_access_token(char *clientIdToken);


// functia de autorizare
// primeste userId si intoarce requestToken
char **
request_authorization_1_svc(char **argp, struct svc_req *rqstp)
{
	static char *result;
	bool found = false;
	result = (char *)malloc(50);

	printf("BEGIN %s AUTHZ\n", *argp);
	for (int i = 0; i < users.size(); i++)
	{
		//cauta userul dupa id in baza de date
		if (strcmp(users[i].userId, *argp) == 0)
		{
			// cand il gaseste ii genereaza requestToken
			strcpy(result, generate_access_token(*argp));
			strcpy(users[i].tokens.requestToken, result);
			found = true;
			break;
		}
	}
	if (!found)
	{
		strcpy(result, "USER_NOT_FOUND");
	}
	else
	{
		// dupa ce a fost generat requestToken
		string line, token, resource, permissions;
		if (!getline(inputFile4, line, '\n'))
		{
			//daca s-a terminat de citit fisierul approvals, se intoarce result
			return &result;
		}
		else
		{
			// se citeste pe rand cate o linie din approvals pentru a vedea
			// permisiunile userului aferente request-ului curent
			istringstream lineStream(line);
			int i = 2;
			vector<struct resourcesPerm> perms;
			while (getline(lineStream, token, ','))
			{
				switch (i % 2)
				{
				case 0:
					resource = token;
					break;
				case 1:
					permissions = token;
					struct resourcesPerm rp;
					rp.resource = (char *)malloc(50);
					rp.permissions = (char *)malloc(50);
					strcpy(rp.resource, resource.c_str());
					strcpy(rp.permissions, permissions.c_str());
					perms.push_back(rp);
					break;
				default:
					break;
				}
				i++;
			}
			// retinem vectorul de resurse si permisiuni 
			// intr-un map la cheia requestToken 
			string key(result);
			approvals[key] = perms;
			printf("  RequestToken = %s\n", result);
		}
	}
	return &result;
}

// functia de access
// primeste struct userPair
// struct userPair {
// 	char *userId;
// 	char *requestToken;
// 	bool_t refreshToken;
// 	bool_t beginRefresh;
// };
struct tokensPair *
request_access_token_1_svc(struct userPair *argp, struct svc_req *rqstp)
{
	static struct tokensPair result;
	result.requestToken = (char *)malloc(50);
	result.accessToken = (char *)malloc(50);
	result.refreshToken = (char *)malloc(50);
	result.error = (char *)malloc(50);

	for (int i = 0; i < users.size(); i++)
	{
		// se cauta dupa id userul
		if (strcmp(users[i].userId, argp->userId) == 0)
		{
			// se verifica daca a fost validat requestToken (permisiuni != *-)
			if (users[i].validatedToken == true)
			{
				users[i].tokens.accessToken = (char *)malloc(50);
				users[i].tokens.refreshToken = (char *)malloc(50);

				// se genereaza accessToken
				char *accessToken = (char *)malloc(50);
				strcpy(accessToken, generate_access_token(argp->requestToken));
				char *refreshToken = (char *)malloc(50);

				// daca se face refresh automat
				if (argp->refreshToken && users[i].tokens.valability == 0 
						&& argp->beginRefresh == true)
				{
					printf("BEGIN %s AUTHZ REFRESH\n", argp->userId);
				}
				strcpy(users[i].tokens.accessToken, accessToken);
				users[i].tokens.valability = valability;
				result.valability = valability;

				strcpy(result.accessToken, accessToken);
				printf("  AccessToken = %s\n", result.accessToken);
				// daca avem request 1, se genereaza si refreshToken
				if (argp->refreshToken)
				{
					strcpy(refreshToken, generate_access_token(accessToken));
					strcpy(users[i].tokens.refreshToken, refreshToken);
					strcpy(result.refreshToken, refreshToken);
					printf("  RefreshToken = %s\n", result.refreshToken);
				}
				free(accessToken);
				free(refreshToken);
				break;
			}
			else
			{
				// daca nu a fost validat requestToken
				strcpy(result.error, "REQUEST_DENIED");
				break;
			}
		}
	}

	return &result;
}

// functia care realizeaza operatiile asupra resurselor
// primeste struct handleResource
// struct handleResource {
// 	char *operation;
// 	char *resource;
// 	char *accessToken;
// };
char **
validate_delegated_action_1_svc(struct handleResource *argp, struct svc_req *rqstp)
{
	static char *result;
	result = (char *)malloc(50);
	strcpy(result, "\0");
	bool found = false;
	int valability = 0;
	char accessToken[50];

	// se cauta user-ul dupa accessToken
	for (int i = 0; i < users.size(); i++)
	{
		bool ok = false;
		if (strcmp(users[i].tokens.accessToken, argp->accessToken) == 0
		 	&& strlen(argp->accessToken) != 0)
		{
			strcpy(accessToken, argp->accessToken);
			found = true;
			// daca token-ul a fost validat
			if (users[i].validatedToken == true)
			{
				// daca token-ul nu mai are valabilitate
				if (users[i].tokens.valability == 0)
				{
					printf("DENY (%s,%s,%s,%d)\n", argp->operation, argp->resource, 
							"", users[i].tokens.valability);
					strcpy(result, "TOKEN_EXPIRED");
					break;
				}
				else
				{
					// daca tokenul mai are valabilitate
					// se cauta daca exista resursa in vectorul resources
					string res(argp->resource);
					vector<string>::iterator it;
					it = find(resources.begin(), resources.end(), res);
					if (it != resources.end())
					{
						// daca exista resursa
						for (int j = 0; j < approvals[users[i].tokens.requestToken].size(); j++)
						{
							// se cauta permisiunile acesteia in map-ul approvals
							if (strcmp(approvals[users[i].tokens.requestToken][j].resource, 
									argp->resource) == 0)
							{
								// se extrag permisiunile din approvals
								string perms(approvals[users[i].tokens.requestToken][j].permissions);
								string op(argp->operation);
								// se extrage prima litera din operatie (R din READ)
								// dar la execute se extrage a 2 a litera (X)
								if (perms.find(op.at(strcmp(argp->operation, "EXECUTE") == 0 
										? 1 : 0)) != string::npos)
								{
									// permission granted daca sunt corecte permisiunile
									users[i].tokens.valability--;
									valability = users[i].tokens.valability;
									printf("PERMIT (%s,%s,%s,%d)\n", argp->operation, 
										argp->resource, accessToken, users[i].tokens.valability);
									strcpy(result, "PERMISSION_GRANTED");
									ok = true;
									break;
								}
								else
								{
									// altfel, operatia nu este permisa
									users[i].tokens.valability--;
									valability = users[i].tokens.valability;
									printf("DENY (%s,%s,%s,%d)\n", argp->operation, 
										argp->resource, accessToken, users[i].tokens.valability);
									strcpy(result, "OPERATION_NOT_PERMITTED");
									ok = true;
									break;
								}
							}
						}
						if (ok)
						{
							break;
						}
					}
					else
					{
						// daca nu s-a gasit resursa in vectorul resources
						users[i].tokens.valability--;
						valability = users[i].tokens.valability;
						printf("DENY (%s,%s,%s,%d)\n", argp->operation, argp->resource, 
							valability == 0 ? "" : accessToken, users[i].tokens.valability);
						strcpy(result, "RESOURCE_NOT_FOUND");
						break;
					}
				}
			}
		}
	}
	if (found == false)
	{
		// daca nu a fost gasit dupa accessToken userul
		printf("DENY (%s,%s,%s,%d)\n", argp->operation, argp->resource, 
			"", valability);
		strcpy(result, "PERMISSION_DENIED");
	}
	else
	{
		// daca userul a fost gasit, dar tokenul a fost invalidat
		if (strlen(result) == 0)
		{
			for (int i = 0; i < users.size(); i++)
			{
				if (strcmp(users[i].tokens.accessToken, argp->accessToken) == 0)
				{
					users[i].tokens.valability--;
					valability = users[i].tokens.valability;
				}
			}
			printf("DENY (%s,%s,%s,%d)\n", argp->operation, argp->resource, 
				accessToken, valability);
			strcpy(result, "OPERATION_NOT_PERMITTED");
		}
	}

	return &result;
}


// functia ce aproba requestToken
char **
approve_request_token_1_svc(char **argp, struct svc_req *rqstp)
{
	static char *result;
	result = (char *)malloc(50);
	bool found = false;

	// se parcurge map-ul ce are cheia requestToken si valoarea
	// un vector de structuri ce contine resurse si permisiunile acestora
	for (auto entry : approvals)
	{
		string key(entry.first);
		const vector<resourcesPerm> value = entry.second;
		string s(*argp);
		// daca cheia este egala cu requestToken-ul curent
		// si permisiunile sunt *-
		if (s == key && strcmp(value[0].resource, "*") == 0 
			&& strcmp(value[0].permissions, "-") == 0)
		{
			// se cauta user-ul in baza de date
			for (int j = 0; j < users.size(); j++)
			{
				if (strcmp(users[j].tokens.requestToken, *argp) == 0)
				{
					// si se invalideaza token-ul
					users[j].validatedToken = false;
					found = true;
					break;
				}
			}
		}
		if (found)
		{
			break;
		}
	}
	if (!found)
	{
		// altfel, se cauta userul si i se valideaza token-ul
		for (int j = 0; j < users.size(); j++)
		{
			if (strcmp(users[j].tokens.requestToken, *argp) == 0)
			{
				users[j].validatedToken = true;
				break;
			}
		}
	}
	strcpy(result, *argp);

	return &result;
}

// functie ce returneaza valabilitatea ramasa a unui token
int *check_valability_1_svc(char **argp, struct svc_req *rqstp)
{
	static int result;

	for (int i = 0; i < users.size(); i++)
	{
		if (strcmp(users[i].userId, *argp) == 0)
		{
			result = users[i].tokens.valability;
			break;
		}
	}

	return &result;
}