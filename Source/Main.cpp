import fab.CoreConfig;
import fab.CoreContent;
import fab.ConfigHotkey;
import fab.DynamicContent;
import fab.FUtil;
import fab.FWindow;
import fab.TitleScreen;
import sdl.SDLBase; 
import sdl.SDLBatchRenderPass;
import sdl.SDLRunner;
import std;

using namespace fab;

int main()
{
	if (!sdl::runner::init()) {
		return -1;
	}

	CoreConfig cfg = CoreConfig(fab::futil::FBC);
	cfg.fullLoad();
	CoreContent cct = DynamicContent::generate(cfg, fab::futil::FBC);

	FWindow window = FWindow(cct);
	window.openScreen(std::make_unique<fab::TitleScreen>(window, cct));

	while(sdl::runner::poll())
	{
		window.update();
		window.render();
		
		// Only limit frame if vsync is off
		if (!cfg.graphicsVSync.get()) {
			sdl::runner::capFrame();
		}
	}

	window.dispose();
	cct.dispose();
	sdl::runner::quit();
	return 0;
}
