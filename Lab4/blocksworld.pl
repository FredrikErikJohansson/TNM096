% TASK B

:- use_module(library(clpfd)).
:- style_check(-singleton).

% actions
act( pick_block_from_table(X),                   % action name
     [block(X), handempty,  clear(X), on(X,0)],  % preconditions
     [handempty, on(X, 0)],                      % delete
     [holding(X)]                                % add
     ).

    act( pick_pyramid_from_table(X),
    [pyramid(X), handempty,  clear(X), on(X,0)],
    [handempty, on(X, 0)],
    [holding(X)]
    ).

act( pickup_block_from_block(X,Y),
     [block(X), handempty, clear(X), on(X,Y), block(Y)],
     [handempty, on(X, Y)],
     [holding(X), clear(Y)]
     ):-
        X #\= Y.

act( pickup_pyramid_from_block(X,Y),
    [pyramid(X), handempty, clear(X), on(X,Y), block(Y)],
    [handempty, on(X, Y)],
    [holding(X), clear(Y)]
    ):-
        X #\= Y.

act( putdown_block_on_table(X),
     [block(X), holding(X)],
     [holding(X)],
     [handempty, on(X,0)]
     ).

act( putdown_pyramid_on_table(X),
    [pyramid(X), holding(X)],
    [holding(X)],
    [handempty, on(X,0)]
    ).


act(  putdown_block_on_block(X,Y),
     [block(X), holding(X), block(Y), clear(Y)],
     [holding(X), clear(Y)],
     [handempty, on(X,Y)]
     ):-
        X #\= Y.

act(  putdown_pyramid_on_block(X,Y),
     [pyramid(X), holding(X), block(Y), clear(Y)],
     [holding(X), clear(Y)],
     [handempty, on(X,Y)]
     ):-
        X #\= Y.


goal_state( [ green(Y),blue(Z),on(X,Y),on(Y,Z) ]).

initial_state(
     [      
            clear(1),
            clear(2),
            clear(3),
            clear(5),

            orange(1),
            orange(4),
            green(2),
            green(5),
            blue(3),
            blue(6),
            
            on(1,0),
            on(2,0),
            on(3,4),
            on(4,0),
            on(5,6),
            on(6,0),

            handempty,
            block(2),
            block(3),
            block(4),
            block(6),
            pyramid(1),
            pyramid(5)
     ]).

