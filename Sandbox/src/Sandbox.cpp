#define GF_PLATFORM_WINDOWS

#include<Graffiti.h>

class Sanbox :public Graffiti::Application {
	public:
		Sanbox() {

		}
		~Sanbox() {

		}
};

Graffiti::Application* Graffiti::CreateApplication() {
	return new Sanbox();
}

