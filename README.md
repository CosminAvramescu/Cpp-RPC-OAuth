<!-- PROJECT LOGO -->
<br />
<div align="center">
  <p align="center">
    <br />
    <a href="https://github.com/othneildrew/Best-README-Template">View Demo</a>
  </p>
</div>

<!-- ABOUT THE PROJECT -->
# Cpp-RPC-Oauth
![oauth](https://i.imgur.com/8YiiFGq.png)

---General aspects

    After running rpcgen, the files I modified are: tema_server.cpp, tema_client.cpp, tema_svc.cpp, tema.h
    and Makefile.

    --- tema.h
	Here we have libraries and defined global variables. We used these global variables in 
    tema_svc.cpp (for initialization, when reading input files) and in tema_server.cpp (for 
    updating values, working with the database).

    --- tema_svc.cpp
	Here I open the files given as a parameters when running (userIds, resources). Also, I initialize 
    the global variable valability, which is also received as an argument. 

--- tema_server.cpp

	Here I implement the server functions (request_authorization_1, request_access_token_1, 
    validate_delegated_action_1,approve_request_token_1_svc). I also implemented check_valability_1. I 
    needed this because I needed to know in the client if the token needed to be automatically refreshed 
    (if the token validity reached 0). Client doesn't have access to the users database, and I needed to 
    get the validity information from the server (so that's why I implemented this function).
    
    --- request_authorization_1
    	Here I get userId and search the users database until I find the user I am interested in. If the
    user is not found, it returns the result USER_NOT_FOUND. Then I generate a token (requestToken) and 
    put it in the result. After the requestToken has been generated, a line is read from the file 
    approvals file to see what permissions the current user has. Save to the requestToken key in the 
    approvals map the stub vector with the resource and permissions read. Then return the result.

    --- request_access_token_1
        It receives userId, requestToken and 2 bool variables. We needed the bool variable refreshToken 
    variable to know whether to generate refreshToken or not. We needed the bool variable beginRefresh to 
    know if the token refresh procedure starts or not. Search by id the user in the database, then check 
    if it has the token validated, otherwise REQUEST_DENIED is returned. If the token has been validated, 
    accessToken is generated. If refreshToken is true, refreshToken is also generated. Update the 
    accessToken and refreshToken values in the user database.

    --- validate_delegated_action_1
        The accessToken, operation and resource are received. Search the database for the user by 
    accessToken. If the token has not been validated, return OPERATION_NOT_PERMITTED. Otherwise, check 
    the validity of the token. If 0, return TOKEN_EXPIRED, otherwise check if the user has permissions to 
    perform the desired operation. If there is no resource, return RESOURCE_NOT_FOUND, otherwise check the 
    permissions vector in approvals to see if there are permissions for the requested resource. Extract 
    the permissions from approvals and compare with the first letter of the operation (for example if we 
    have RMD permissions and READ operation, we will check if the first letter of READ - R is in RMD). 
    Only at execute we check the 2nd letter (X). If there are permissions, it returns PERMISSION_GRANTED, 
    otherwise it returns OPERATION_NOT_PERMITTED. If the user's accessToken was not found in the database, 
    return PERMISSION_DENIED. 

    --- approve_request_token_1_svc        
	Browse the approvals map to see if the requestToken key contains the permissions *-. If these 
    permissions are found, invalidate the token via the validity variable in the users database. Then 
    return requestToken.

--- tema_client.cpp

    	We used 3 global variables, userData (map between userId and accessToken - to extract accessToken 
    and send it to validate_delegated_action_1), userRefresh (map between userId and refreshToken - to 
    send refreshToken as requestToken to request_access_token_1 for automatic refresh) and automatedRefresh
    (map between userId and bool - to know if token refresh is done or not).
    
     --- authz_and_access
        I made this function to remove duplicate code, because in both cases (REQUEST 0 and REQUEST 1) the 
    functions authorize, approve and access are called from the server. Here we build the structures for 
    each function call and give them as arguments to the server. Then the userData, userRefresh and 
    automatedRefresh maps are updated with the new values obtained and the values of the tokens obtained 
    are displayed. 
    
     --- oauth_1
        Read line by line from client.in and execute the operations. If we have REQUEST 0 or 1, the 
    authz_and_access is called. Then the validity of the current user token is checked, and if validity 
    is 0 and the user has automatic renewal, then call request_access_token_1 method and update again the 
    values in userData, userRefresh maps. If the operation is not of type Request (i.e. READ, DELETE etc), 
    then build the structure and call the method validate_delegated_action_1.



### Built With

This section should list any major frameworks/libraries used to bootstrap your project. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples.

* [![Next][Next.js]][Next-url]
* [![React][React.js]][React-url]
* [![Vue][Vue.js]][Vue-url]
* [![Angular][Angular.io]][Angular-url]
* [![Svelte][Svelte.dev]][Svelte-url]
* [![Laravel][Laravel.com]][Laravel-url]
* [![Bootstrap][Bootstrap.com]][Bootstrap-url]
* [![JQuery][JQuery.com]][JQuery-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Prerequisites

This is an example of how to list things you need to use the software and how to install them.
* npm
  ```sh
  npm install npm@latest -g
  ```

### Installation

_Below is an example of how you can instruct your audience on installing and setting up your app. This template doesn't rely on any external dependencies or services._

1. Get a free API Key at [https://example.com](https://example.com)
2. Clone the repo
   ```sh
   git clone https://github.com/your_username_/Project-Name.git
   ```
3. Install NPM packages
   ```sh
   npm install
   ```
4. Enter your API in `config.js`
   ```js
   const API_KEY = 'ENTER YOUR API';
   ```

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Use this space to show useful examples of how a project can be used. Additional screenshots, code examples and demos work well in this space. You may also link to more resources.

_For more examples, please refer to the [Documentation](https://example.com)_

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ROADMAP -->
## Roadmap

- [x] Add Changelog
- [x] Add back to top links
- [ ] Add Additional Templates w/ Examples
- [ ] Add "components" document to easily copy & paste sections of the readme
- [ ] Multi-language Support
    - [ ] Chinese
    - [ ] Spanish

See the [open issues](https://github.com/othneildrew/Best-README-Template/issues) for a full list of proposed features (and known issues).

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTRIBUTING -->
## Contributing

Contributions are what make the open source community such an amazing place to learn, inspire, and create. Any contributions you make are **greatly appreciated**.

If you have a suggestion that would make this better, please fork the repo and create a pull request. You can also simply open an issue with the tag "enhancement".
Don't forget to give the project a star! Thanks again!

1. Fork the Project
2. Create your Feature Branch (`git checkout -b feature/AmazingFeature`)
3. Commit your Changes (`git commit -m 'Add some AmazingFeature'`)
4. Push to the Branch (`git push origin feature/AmazingFeature`)
5. Open a Pull Request

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `LICENSE.txt` for more information.

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- CONTACT -->
## Contact

Your Name - [@your_twitter](https://twitter.com/your_username) - email@example.com

Project Link: [https://github.com/your_username/repo_name](https://github.com/your_username/repo_name)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- ACKNOWLEDGMENTS -->
## Acknowledgments

Use this space to list resources you find helpful and would like to give credit to. I've included a few of my favorites to kick things off!

* [Choose an Open Source License](https://choosealicense.com)
* [GitHub Emoji Cheat Sheet](https://www.webpagefx.com/tools/emoji-cheat-sheet)
* [Malven's Flexbox Cheatsheet](https://flexbox.malven.co/)
* [Malven's Grid Cheatsheet](https://grid.malven.co/)
* [Img Shields](https://shields.io)
* [GitHub Pages](https://pages.github.com)
* [Font Awesome](https://fontawesome.com)
* [React Icons](https://react-icons.github.io/react-icons/search)

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/othneildrew/Best-README-Template.svg?style=for-the-badge
[contributors-url]: https://github.com/othneildrew/Best-README-Template/graphs/contributors
[forks-shield]: https://img.shields.io/github/forks/othneildrew/Best-README-Template.svg?style=for-the-badge
[forks-url]: https://github.com/othneildrew/Best-README-Template/network/members
[stars-shield]: https://img.shields.io/github/stars/othneildrew/Best-README-Template.svg?style=for-the-badge
[stars-url]: https://github.com/othneildrew/Best-README-Template/stargazers
[issues-shield]: https://img.shields.io/github/issues/othneildrew/Best-README-Template.svg?style=for-the-badge
[issues-url]: https://github.com/othneildrew/Best-README-Template/issues
[license-shield]: https://img.shields.io/github/license/othneildrew/Best-README-Template.svg?style=for-the-badge
[license-url]: https://github.com/othneildrew/Best-README-Template/blob/master/LICENSE.txt
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/othneildrew
[product-screenshot]: images/screenshot.png
[Next.js]: https://img.shields.io/badge/next.js-000000?style=for-the-badge&logo=nextdotjs&logoColor=white
[Next-url]: https://nextjs.org/
[React.js]: https://img.shields.io/badge/React-20232A?style=for-the-badge&logo=react&logoColor=61DAFB
[React-url]: https://reactjs.org/
[Vue.js]: https://img.shields.io/badge/Vue.js-35495E?style=for-the-badge&logo=vuedotjs&logoColor=4FC08D
[Vue-url]: https://vuejs.org/
[Angular.io]: https://img.shields.io/badge/Angular-DD0031?style=for-the-badge&logo=angular&logoColor=white
[Angular-url]: https://angular.io/
[Svelte.dev]: https://img.shields.io/badge/Svelte-4A4A55?style=for-the-badge&logo=svelte&logoColor=FF3E00
[Svelte-url]: https://svelte.dev/
[Laravel.com]: https://img.shields.io/badge/Laravel-FF2D20?style=for-the-badge&logo=laravel&logoColor=white
[Laravel-url]: https://laravel.com
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
