#include "Game.h"

//struct Allocate {
//	uint32_t TotalAl = 0;
//	uint32_t TotalFree = 0;
//	uint32_t CurUsage() { return TotalAl - TotalFree; };
//};
//static Allocate Set;
//void* operator new(size_t size)
//{
//	Set.TotalAl += size;
//	return malloc(size);
//}
//void operator delete(void* memory, size_t size)
//{
//	Set.TotalFree += size;
//	free(memory);
//}
//static void PrintMem()
//{
//	std::cout << "Mem" << Set.CurUsage() << "\n";
//}
int main()
{
	glm::vec3 SkyColor = glm::vec3(0.f, 0.f, 1.f);
	Game game("Test Map",1920,1080,4,6,false,SkyColor);

	//// MAIN LOOP
	while (!game.getWindowShouldClose())
	{
		game.update();
		game.render();
		//PrintMem();
	}

	return 0;
}