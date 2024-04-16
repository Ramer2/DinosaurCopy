# Dinosaur Copy
**This may become not a dinosaur game, but something else**

**THIS IS A COPY**

This is my try to create a Dinosaur game copy for my C++ classes at my university. I do not have any relation to the original game.
## How this game works?
This whole project is based on the SFML library.
The core mechanics are the same as for the Google Dinosaur. There's an entity, that a player can jump with to avoid obsticles.

If a player hits an obsticle, the game is lost.
The game is infinite.

### Shortcuts
- To implement infinite "map" i decided to make the entity static and cacti moving (so, it's not the entity that's moving, it's cacti, that are running at this entity. Sounds like fun, right?)
- Jumping is just giving the entity vertical speed with negative acceleration
- There are a total of three cacti objects in the game. So, it's only three cacti, that are constantly running at you, even better.
When one cactus leaves the left side of the window, it is moved to the right (also outside the window).

# Conclusion
This is a very fun project, that doesn't take a lot of time (the core functionality took me 2hrs). I learned a lot, but the main thing I understood is: **PRAISE THE VECTOR GOD** 
(I would've lost my sanity, if I were to write all the moving around using x and y floats alone).

## Thank you for stopping by)
