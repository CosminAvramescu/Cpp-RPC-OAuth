# Cpp-RPC-Oauth
![oauth](https://i.imgur.com/8YiiFGq.png)

---General aspects

    After running rpcgen, the files I modified are: tema_server.cpp, tema_client.cpp, tema_svc.cpp, tema.h and Makefile.

    --- tema.h
	Here we have libraries and defined global variables. We used these global variables in tema_svc.cpp (for initialization, when reading input files) 
and in tema_server.cpp (for updating values, working with the database).

    --- tema_svc.cpp
	Here I open the files given as a parameters when running (userIds, resources). Also, I initialize the global variable valability, which is also received as an argument. 

--- tema_server.cpp 
	Here I implement the server functions (request_authorization_1, request_access_token_1, validate_delegated_action_1,approve_request_token_1_svc).
    I also implemented check_valability_1. I needed this because I needed to know in the client if the token needed to be automatically refreshed (if the token validity reached 0). 
    Client doesn't have access to the users database, and I needed to get the validity information from the server (so that's why I implemented this function).
    
    --- request_authorization_1
    	Here I get userId and search the users database until I find the user I am interested in.
    If the user is not found, it returns the result USER_NOT_FOUND. Then I generate a token (requestToken) and put it in the result. 
    After the requestToken has been generated, a line is read from the file approvals file to see what permissions the current user has. 
    Save to the requestToken key in the approvals map the stub vector with the resource and permissions read. Then return the result.

    --- request_access_token_1
        It receives userId, requestToken and 2 bool variables. We needed the bool variable refreshToken variable to know whether to generate refreshToken or not.
    We needed the bool variable beginRefresh to know if the token refresh procedure starts or not. Search by id the user in the database, then check if it has the 
    token validated, otherwise REQUEST_DENIED is returned. If the token has been validated, accessToken is generated. If refreshToken is true, 
    refreshToken is also generated. Update the accessToken and refreshToken values in the user database.

    --- validate_delegated_action_1
        The accessToken, operation and resource are received. Search the database for the user by accessToken.
    If the token has not been validated, return OPERATION_NOT_PERMITTED. Otherwise, check the validity of the token. If 0, return TOKEN_EXPIRED, otherwise check if 
    the user has permissions to perform the desired operation. If there is no resource, return RESOURCE_NOT_FOUND, 
    otherwise check the permissions vector in approvals to see if there are permissions for the requested resource. Extract the permissions from approvals and compare 
    with the first letter of the operation (for example if we have RMD permissions and READ operation, we will check if the first letter of READ - R is in RMD). 
    Only at execute we check the 2nd letter (X). If there are permissions, it returns
    PERMISSION_GRANTED, otherwise it returns OPERATION_NOT_PERMITTED. If the user's accessToken was not found in the database, return PERMISSION_DENIED. 

    --- approve_request_token_1_svc        
	Browse the approvals map to see if the requestToken key contains the permissions *-. If these permissions are found, invalidate the token via 
    the validity variable in the users database. Then return requestToken.

--- tema_client.cpp
    	We used 3 global variables, userData (map between userId and accessToken - to extract accessToken and send it to validate_delegated_action_1), 
     userRefresh (map between userId and refreshToken - to send refreshToken as requestToken to request_access_token_1 for automatic refresh) 
     and automatedRefresh (map between userId and bool - to know if token refresh is done or not).
    
     --- authz_and_access
        I made this function to remove duplicate code, because in both cases (REQUEST 0 and
    REQUEST 1) the functions authorize, approve and access are called from the server. Here we build the structures for each function call and give them as 
    arguments to the server. Then the userData, userRefresh and automatedRefresh maps are updated with the new values obtained and the values of the tokens obtained are displayed. 
    
     --- oauth_1
        Read line by line from client.in and execute the operations. If we have REQUEST 0 or 1, the
    authz_and_access is called. Then the validity of the current user token is checked, and if
    validity is 0 and the user has automatic renewal, then call request_access_token_1 method and update again the values in userData, userRefresh maps. 
    If the operation is not of type Request (i.e. READ, DELETE etc), then build the structure and call the method validate_delegated_action_1.