#include "engine.h"
#include <thread>

int main(int argc, char* argv[]) {
	vector<std::string> argvString;
	for (int i = 0; i < argc; i++) {
		argvString.push_back({ argv[i] });
	}

	engine_t engine(argvString);
	engine.window.setActive(false);

	std::thread renderThread([&]() {
		engine.window.setActive(true);
		while (engine.window.isOpen()) {
			if (engine.getExitFlag())
				break;
			engine.window.clear();
			engine.drawUI();
			engine.window.display();
		}
		engine.window.close();
	});

	engine.start();
	renderThread.join();

	return 0;
}
