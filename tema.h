/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEMA_H_RPCGEN
#define _TEMA_H_RPCGEN

#include <rpc/rpc.h>
#include <sstream>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif

// structura care face parte din cea pentru baza de date cu useri 
// de asemenea, aceasta este returnata de functia request_access_token_1_svc
struct tokensPair {
	char *requestToken;
	char *accessToken;
	char *refreshToken;
	int valability;
	char *error;
};
typedef struct tokensPair tokensPair;

// structura pentru baza de date cu useri
struct user {
	char *userId;
	struct tokensPair tokens;
	bool_t validatedToken;
};
typedef struct user user;

// structura care se trimite ca argument la validate_delegated_action_1
struct handleResource {
	char *operation;
	char *resource;
	char *accessToken;
};
typedef struct handleResource handleResource;

// structura care se trimite la request_access_token_1
struct userPair {
	char *userId;
	char *requestToken;
	bool_t refreshToken;
	bool_t beginRefresh;
};
typedef struct userPair userPair;

// structura pentru vectorul de resurse si permisiuni din map-ul approvals
struct resourcesPerm {
	char *resource;
	char *permissions;
};
typedef struct resourcesPerm resourcesPerm;

// variabilele externe folosite de server
extern vector<user> users;
extern vector<string> resources;
extern map<string, vector<struct resourcesPerm>> approvals;
extern int valability;
extern ifstream inputFile4;

#define OAUTH 0x31234567
#define OAUTHVERS 1

#if defined(__STDC__) || defined(__cplusplus)
#define request_authorization 1
extern  char ** request_authorization_1(char **, CLIENT *);
extern  char ** request_authorization_1_svc(char **, struct svc_req *);
#define request_access_token 2
extern  struct tokensPair * request_access_token_1(struct userPair *, CLIENT *);
extern  struct tokensPair * request_access_token_1_svc(struct userPair *, struct svc_req *);
#define validate_delegated_action 3
extern  char ** validate_delegated_action_1(struct handleResource *, CLIENT *);
extern  char ** validate_delegated_action_1_svc(struct handleResource *, struct svc_req *);
#define approve_request_token 4
extern  char ** approve_request_token_1(char **, CLIENT *);
extern  char ** approve_request_token_1_svc(char **, struct svc_req *);
#define check_valability 5
extern  int * check_valability_1(char **, CLIENT *);
extern  int * check_valability_1_svc(char **, struct svc_req *);
extern int oauth_1_freeresult (SVCXPRT *, xdrproc_t, caddr_t);

#else /* K&R C */
#define request_authorization 1
extern  char ** request_authorization_1();
extern  char ** request_authorization_1_svc();
#define request_access_token 2
extern  struct tokensPair * request_access_token_1();
extern  struct tokensPair * request_access_token_1_svc();
#define validate_delegated_action 3
extern  char ** validate_delegated_action_1();
extern  char ** validate_delegated_action_1_svc();
#define approve_request_token 4
extern  char ** approve_request_token_1();
extern  char ** approve_request_token_1_svc();
#define check_valability 5
extern  int * check_valability_1();
extern  int * check_valability_1_svc();
extern int oauth_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_tokensPair (XDR *, tokensPair*);
extern  bool_t xdr_user (XDR *, user*);
extern  bool_t xdr_handleResource (XDR *, handleResource*);
extern  bool_t xdr_userPair (XDR *, userPair*);
extern  bool_t xdr_resourcesPerm (XDR *, resourcesPerm*);

#else /* K&R C */
extern bool_t xdr_tokensPair ();
extern bool_t xdr_user ();
extern bool_t xdr_handleResource ();
extern bool_t xdr_userPair ();
extern bool_t xdr_resourcesPerm ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEMA_H_RPCGEN */
