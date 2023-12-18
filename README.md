# Social Gaming

----

A CMPT373 project for Fall 2023.


To run the example server
```
bin/chatserver 8000 ../social-gaming/webchat.html
```

To run the example client
```
bin/client-ncurses localhost 8000
```

To run the server
```
bin/gameServer 8000 ../social-gaming/webchat.html
```

To run the client
```
bin/gameClient localhost 8000
```

To run the tests
```
./bin/runAllTests
```

To Run a Suite of Tests
```
Fill in the ""
./bin/runAllTests --gtest_filter="NAME_OF_TEST_SUITE".*
```

To run a specific Test
```
Fill in the ""
./bin/runAllTests --gtest_filter="NAME_OF_TEST_SUITE"."NAME_OF_TEST"
```
