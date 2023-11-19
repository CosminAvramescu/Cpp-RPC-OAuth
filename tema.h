/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#ifndef _TEMA_H_RPCGEN
#define _TEMA_H_RPCGEN

#include <rpc/rpc.h>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

#ifdef __cplusplus
extern "C" {
#endif


struct tokensPair {
	char *accessToken;
	char *refreshToken;
	int valability;
};
typedef struct tokensPair tokensPair;

struct user {
	char *userId;
	struct tokensPair tokens;
	bool_t validatedToken;
};
typedef struct user user;

struct handleResource {
	char *operation;
	char *resource;
	char *accessToken;
};
typedef struct handleResource handleResource;

struct userPair {
	char *userId;
	char *accessToken;
};
typedef struct userPair userPair;

extern vector<user> users;

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
extern int oauth_1_freeresult ();
#endif /* K&R C */

/* the xdr functions */

#if defined(__STDC__) || defined(__cplusplus)
extern  bool_t xdr_tokensPair (XDR *, tokensPair*);
extern  bool_t xdr_user (XDR *, user*);
extern  bool_t xdr_handleResource (XDR *, handleResource*);
extern  bool_t xdr_userPair (XDR *, userPair*);

#else /* K&R C */
extern bool_t xdr_tokensPair ();
extern bool_t xdr_user ();
extern bool_t xdr_handleResource ();
extern bool_t xdr_userPair ();

#endif /* K&R C */

#ifdef __cplusplus
}
#endif

#endif /* !_TEMA_H_RPCGEN */
