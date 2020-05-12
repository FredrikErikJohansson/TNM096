% TASK A

act( go(X,Y),                               % action name
     [in(X), on(floor), connect(X,Y)],      % preconditions
     [in(X)],                               % delete
     [in(Y)]                                % add
).

act( push(B, X, Y),
     [box(B), on(floor), in(X), in(B,X), connect(X,Y), connect(Light, X), light(Light, on)],
     [in(X), in(B,X)],
     [in(Y), in(B,Y)]
).

act( climbUp(B),
     [box(B), on(floor), in(X), in(B,X)],
     [on(floor)],
     [on(B)]
).

act( climbDown(B),
     [box(B), on(B), in(X), in(B,X)],
     [on(B)],
     [on(floor)]
).

act( turnOn(L),
     [light(L,off), on(B), in(L), in(B,L)],
     [light(L,off)],
     [light(L,on)]
).

act( turnOff(L),
     [light(L,on), on(B), in(L), in(B,L)],
     [light(L,on)],
     [light(L,off)]
).
     
goal_state( [in(room1)] ).
%goal_state( [light(switch1, off)]).
%goal_state( [in(box2, room2)]).
%goal_state([in(room1),light(switch1, off),in(box2, room2)]).

initial_state([
               % Shakey
               in(room3),
               on(floor),
               
               % Boxes
               box(box1),
               box(box2),
               box(box3),
               box(box4),

               % Rooms
               room(room1),
               room(room2),
               room(room3),
               room(room4),
               room(corridor),
               
               % Box locations
               in(box1, room1),
               in(box2, room1),
               in(box3, room1),
               in(box4, room1),
               
               % Lights
               light(switch1, on),
               light(switch2, off),
               light(switch3, off),
               light(switch4, on),
               light(alwaysOn, on),
               
               % Connect lights to rooms
               connect(switch1, room1),
               connect(switch2, room2),
               connect(switch3, room3),
               connect(switch4, room4),
               connect(alwaysOn, corridor),
               
               connect(room1, switch1),
               connect(room2, switch2),
               connect(room3, switch3),
               connect(room4, switch4),
               
               % Connect rooms to corridor
               connect(corridor, room1),
               connect(corridor, room2),
               connect(corridor, room3),
               connect(corridor, room4),
               
               connect(room1, corridor),
               connect(room2, corridor),
               connect(room3, corridor),
               connect(room4, corridor)
]).