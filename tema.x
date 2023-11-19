struct tokensPair {
	string accessToken<>;
	string refreshToken<>;
    int valability;
    string error<>;
};
struct user {
	string userId<>;
    struct tokensPair tokens;
	bool validatedToken;
};
struct handleResource{
    string operation<>;
    string resource<>;
    string accessToken<>;
};
struct userPair{
    string userId<>;
    string accessToken<>;
};
struct resourcesPerm{
    string resource<>;
    string permissions<>;
};
program OAUTH {
    version OAUTHVERS {
        string request_authorization (string userId<>) = 1;
        struct tokensPair request_access_token (struct userPair userData) = 2;
        string validate_delegated_action (struct handleResource resourceOperation) = 3;
        string approve_request_token (string accessToken<>) = 4;
    } = 1;
} = 0x31234567;

