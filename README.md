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

Social Gaming Platform
This semester, you will create a platform for developing social games that can be played amongst friends or strangers. Your focus is on creating the engine that allows users to easily and simply develop social games that are highly customizable. Those of you familiar with video game collections like the Jackbox Party Pack or social games like Coup, Werewolf, Loveletter, or Citadels will already be acquainted with the basic idea.

You will create a game server that can be configured by the person running the server to allow games to be played. The configuration of the server will be specified in JSON. The specification for a single game will be in a custom language called SocialGaming. A game specification will define the state that a game must maintain as well as the rules of the game. You may think of the specifications as being written in a domain specific programming language that your game server will interpret.

When a user connects to the game server, they may either join a game or create a new game amongst those served. When a new game is created, it may be configured by the creating player, who is designated as the “owner” of the game session and will have admin controls over the game. The owner may configure the game and create it, after which they will receive an “invite code”. Other players connecting to the game server may join the game by specifying the invite code. Note, the owner of the game need not be a player. If they wish to play the game, they may also join.

You may imagine that a typical session works as follows. Several friends get together and decide to play a game. One person connects to the game server via their computer and configures and then starts the game. The game runs on that main computer, with the primary screen observable by all players. Separately, each player may log into the server and join the game on their phones. Each individual player can interact with the game solely through their phones. Any player specific instructions or actions will be sent to specific players on their phones, while any global information will be displayed to all players on the main game screen.

To start, this is all you are given. More details on the exact nature of, e.g., the domain specific language for specifying games and other details will be provided over time. Some will be provided sooner, some will be provided later. All may even be incorrect.

Implementation Constraints
The core project including all business logic will run on a server and be written in C++. Users will be able to access the system via web applications that present a simple interactive view. One example of a simple chat room that already provides this behavior has been provided for you. Following this design, matters of presentation are handled on the client. You may be asked to contruct another (different) client as a requirement.

Does this page define your project requirements?
In short, no. This page provides initial directions that should be helpful in planning the first iteration.

The description of the project as presented here is incomplete. It is not intended to be complete. In fact, it may not even be correct or stable. You should expect the requirements of the project to be flexible and change over time as you interact with the customer. If there are deficiencies in your design, your requirements may even change to make the deficiencies hurt more.

Remember that you should iterate toward a solution, demonstrating your progress and asking for concrete feedback about current behavior of the system and desired behavior of the system. In order to do that, you always need a running system. Make that a priority. You can also identify core abstract behaviors and design your system such that concrete changes can be made with marginal burden.

Game Specification Structure
A game specification is defined in a SocialGaming file. A minimal "empty" specification is shown below. A specification defines several data portions that are loosely inspired by JSON. Following those is a single rules section that defines the rules that a game will follow. The data related sections define the general configuration of the game, the constant data used by the game, the global variable state of the game, per player state, and per audience member state.

configuration {
  name: ""
  player range: (0, 0)
  audience: false
  setup: {}
}
constants {}
variables {}
per-player {}
per-audience {}
rules {}
A parser for these specifications has been provided to you using standard parsing libraries common in industry. This parser takes in a text file containing a specification and transforms it into a tree with one node for each element of the specification. A wrapper library that provides a more convenient C++ API for the parser has also been provided.

This "empty" specification defines a game with no players, no audience, no state, and no rules that does nothing. The configuration contains enough information that the game could be selected and started by a game owner, and it would promptly end.

Configuration
The configuration of a game controls general settings that may assist in game creation and game management for the owner. This includes such things as (1) the name of the game type to show when selecting the type of game to create (2) specifying any data that may be uploaded by the owner, e.g. questions and answers for quiz based games, (3) specifying minima and maxima for the number of players as a closed range, and (4) specifying whether additional game joiners beyond the players can participate as audience members.

"setup" defines settings that can or must be configured by the owner of the game upon game creation. This can include such things as the number of rounds to play. For example, I might define a numerical configuration followed by boolean using:

num_rounds {
  kind: integer,
  prompt: "Number of rounds to play",
  range: (1, 10),
  default: 3,
}
public_voting {
  kind: boolean,
  prompt: "Player votes are public",
}
Notice that num_rounds has a default value and need not be set explicitly.

Configuration kinds can be 'boolean', 'integer', 'string', 'enum', 'question-answer', 'multiple-choice', 'json'. The first three are straightforward. An enum requires an owner to select an option from a list of options with descriptions for the user:

mode {
  kind: enum,
  prompt: "Game Style",
  choices: {
    'fast':     'A quick round with friends',
    'standard': 'Standard play',
    'long':     'A marathon battle against former friends',
  }
}
The latter three kinds indicate custom JSON file formats or general JSON files whose data can be uploaded by the game owner upon creation.

Constants, Variables, Players, and the Audience
The constants, variables, per-player, and per-audience sections define state that may be accessed by the game rules during play. The constants and variables sections contain maps from names to values. The per-player and per-audience sections define maps that exist for each player of a game and each audience member. Top level names must be unique. Values may themselves be (1) maps from names to values, (2) lists of values, or (3) literal strings, numbers, or booleans. In the initial version of the language, new top level names/keys within these sections cannot be added by rules. They must be predetermined within the specification.

Note that some additional forms of game state must be accessible via rules, as well. In particular, the "players" name specifies a list of the player data maps, and the "audience" name specifies a list of audience data maps. Every player map also implicitly has a "name" key that maps to the unique name that the player selected upon joining the game.

Rules
Most of the actual game behavior is controlled by the rules section and the semantics of the rules therein. Essentially, these rules define a custom embedded domain specific language for actions that may be taken within a game. Note that it may be possible to validate some basic correctness properties of rules. These rules are subject to change and will be refined via discussion. They do, however, provide an appropriate jumping off point for planning and designing a good solution.

Lists of rules define a sequence of operations in which each rule must be performed in sequential order. Note that apparent parallelism will exist, but no use of explicit parallelism or threads is required as a part of this project. Indeed, I heavily encourage avoiding the use of threads for implementing the interpretation of rules. The different forms of rules are explored within the remaining sections of this document.

In descriptions of the rules below, double angle brackets indicate a description of the content at that place in a rule, for instance << fresh variable >> indicates creating a new variable at a particular location in a rule. Optional syntax will typically be designated with square brackets like [ extra option ].

Control Structures
Control structures determine when and how different groups of instructions may execute. This includes concepts like sequencing, branching, and parallelism. Note that parallelism in this project is only from the perspective of the players. No parallel control structures actually require hardware level parallelism in software design or concurrency level primitives like threads.

For (each element of a list)
  for << fresh variable >> in << expression evaluating to a list >> {
    << statement list >>
  }
General looping
  while << boolean expression >> {
    << statement list >>
  }
Parallel for (each)
  parallel for << fresh variable >> in << expression evaluating to a list >> {
    << statement list >>
  }
Executes the list of statements once for each element in the list, but it does not wait for the processing of a previous element to finish before starting the next element of the list.

Inparallel
  in parallel {
    << statement list >>
  }
This starts executing each statement in a list without waiting for the previous statements in the list to finish. Note that this creates the potential for bugs even in a single threaded scenario depending on your design. It may be refined in the future.

Match
  match << expression >> {
    << expression candidate 1 >> => { << statement list >> }
    << expression candidate 2 >> => { << statement list >> }
    ...
  }
List Operations
List operations define actions on lists. In some cases, an operation may require a named list variable like constants.cards. In other cases, they may simply use list expressions.

Extend
extend << named list >> with << list expression >> ;
This appends the contents of the expression to the named list.

Reverse
reverse << named list >> ;
Shuffle
shuffle << named list >> ;
Sort
sort << named list >> [by << key>>];
By default, sorting is in ascending order. If a key is provided, then elements of the list will be treated as maps, and the key will select an entry of the map to use for sorting.

Deal
deal << integer expression >> to << player list >> from << list expression >>;
The list of all players can be specified via the shorthand all.

Discard
discard << integer expression >> from << named list >>;
Timing
Timer
time for << integer expression >> [ << "at most" or "exactly"] [in << named variable ] {
  << statement list >>
}
A timing block sets a timer in seconds for a list of statements.

If the at most mode is used, then the list of statements will stop executing after time expires. If the exactly mode is used, then the list of statements will both stop executing at the given time and pad the time taken to fill the expected duration.

If in << named variable >> is specified then the given variable will be set to false before the timer has expired and true after the timer has expired.

Human Input
Input choice
  input choice to << player >> {
    prompt: << prompt string >>
    choices: << list expression >>
    target: << named variable in which to store the selection >>
    [timeout: << integer expression >>]
  }
This allows the player to make a multiple choice selection.

Input text
  input text to << player >> {
    prompt: << prompt string >>
    target: << named variable in which to store the selection >>
    [timeout: << integer expression >>]
  }
Input range
  input range to << player >> {
    prompt: << prompt string >>
    range: (<< minimum value >>, << maximum value >>)
    target: << named variable in which to store the selection >>
    [timeout: << integer expression >>]
  }
Input vote
  input vote to << player >> {
    prompt: << prompt string >>
    choices: << list expression >>
    target: << named variable in which to store the selection >>
    [timeout: << integer expression >>]
  }
Output
Message
message << player list >> << string expression >>;
all can be used as a shorthand for the list of all players.

Scores
scores << list of keys >>;
Prints a scoreboard on the global display using the given attribute(s) of each player defined by the key list.

Variables and Expressions
Standard arithmetic operations apply. They may even apply to types other than integers! Literals for lists and maps can be defined in a style similar to JSON.

Assignment
Assignment uses the <- or "stores to" operator, e.g.

x <- [ "a", "literal", "list" ];

Numerical Attributes
Given a number, I can access attributes of the number similar to attributes of a list. For instance, I can treat a number as a list by saying "count.upfrom(1)". That defines the list of numbers 1, 2, …, count.

List Attributes
When given a list name, attributes of a list can be referred to and elements of a list can be sliced to represent new lists. For example, suppose that the following list is defined in the constants:

  roles: [
    { name: "Duke", action: "Tax. The duke may take 3 coins from the treasury."},
    { name: "Assassin", action: "Assassinate. The assassin may force another player to give up an influence."},
    ...
  ]
The name of the list is roles and can be referred to within the rules. The size of the list may be accessed through the attribute roles.size(). Lists may be implicitly created, as well. If the elements of the list are maps, then the keys of the maps define additional lists. For instance, roles.elements.name defines the list of names contained within the above list. Additional useful attributes are contains and collect that perform "any of" checks and "filtering" respectively.

Example
This demonstrates a simple rock paper scissors game. It is a good place to aim initially. Several smaller portions capture individual goals that may be risky for each group.

// These are examples of core properties of a game that might be
// configured when setting up an instance or lobby for a game.
configuration {
  name: "Rock, Paper, Scissors"
  player range: (2, 4)
  audience: false
  setup: {
    rounds {
      kind: integer
      prompt: "The number of rounds to play"
      range: (1, 20)
    }
  }
}

// Global constants can be expressed in a JSON-like format.
constants {
  weapons: [
    { name: "Rock",     beats: "Scissors" },
    { name: "Paper",    beats: "Rock"     },
    { name: "Scissors", beats: "Paper"    },
  ]
}

// The same is true of the initial values for global variables.
variables {
  winners: []
}

// The per player state exists independently for each player, so
// initializing `wins` below to 0 creates a counter for each player and
// initializes it to 0.
per-player {
  wins: 0
}

// If there were some form of audience participation, the state for each
// audience member could also be tracked separately. What sorts of
// considerations might that imply for implementing it?
per-audience {}

// Finally, the rules of the game execute as previously described after the
// game has been started.
rules {
  for round in configuration.rounds.upfrom(1) {

    discard winners.size() from winners;

    message all "Round {round}. Choose your weapon!";

    parallel for player in players {
      input choice to player {
        prompt: "{player.name}, choose your weapon!"
        choices: weapons.name
        target: player.name
        timeout: 10
      }
    }

    for weapon in weapons {
      match !players.elements.weapon.contains(weapon.name) {
        true => {
          extend winners with players.elements.collect(player, player.weapon = weapon.beats);
        }
      }
    }

    match true {
      winners.size() = players.size() || winners.size() = 0 => {
        message all "Tie game!";
      }
      true => {
        message all "Winners: {winners.elements.name}";
        for winner in winners {
          winner.wins <- winner.wins + 1;
        }
      }
    }
  }

  // Report the final scores!
  scores ["wins"];
}

To run a specific Test
```
Fill in the ""
./bin/runAllTests --gtest_filter="NAME_OF_TEST_SUITE"."NAME_OF_TEST"
```
