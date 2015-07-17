#include "RenderSystem.h"
#include <memory>

int main(int argc, const char **argv)         
{       
	std::shared_ptr<byhj::RenderSystem> app = std::make_shared<byhj::RenderSystem>();    

	app->Run(app);  

	return 0;                                     
}