/*   ____.              .__            _____                 __
    |    |____    _____ |__| ____     /     \   ____   _____/  |_  ____   ____   _____   ___________ ___.__.
    |    \__  \  /     \|  |/ __ \   /  \ /  \ /  _ \ /    \   __\/ ___\ /  _ \ /     \_/ __ \_  __ <   |  |
/\__|    |/ __ \|  Y Y  \  \  ___/  /    Y    (  <_> )   |  \  | / /_/  >  <_> )  Y Y  \  ___/|  | \/\___  |
\________(____  /__|_|  /__|\___  > \____|__  /\____/|___|  /__| \___  / \____/|__|_|  /\___  >__|   / ____|
              \/      \/        \/          \/            \/    /_____/              \/     \/       \/
Jamie Montgomery
19/01/2026
*/
#include <iostream>
#include "AStar.h"
#include "TestCases.h"
#include <SFML/Graphics.hpp>

int main() {

	testComplexMaze();
    testNoPath();
	testStartEqualsGoal();
    return 0;
}