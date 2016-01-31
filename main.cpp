#include <SDL2pp/SDL2pp.hh>

using namespace SDL2pp;

int main() {
try {
  using namespace SDL2pp;

  // Init SDL; will be automatically deinitialized when the object is destroyed
  SDL sdl(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

  // Likewise, init SDL_ttf library
  SDLTTF sdl_ttf;

  // Straightforward wrappers around corresponding SDL2 objects
  // These take full care of proper object destruction and error checking
  Window window("libSDL2pp demo",
                SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                640, 480, SDL_WINDOW_RESIZABLE);
  Renderer renderer(window, -1, SDL_RENDERER_ACCELERATED);
  Texture sprite1(renderer, SDL_PIXELFORMAT_ARGB8888,
                            SDL_TEXTUREACCESS_STATIC, 16, 16);
  Texture sprite2(renderer, "sprite.png"); // SDL_image support

  Font font("Vera.ttf", 20); // SDL_ttf font

  // Initialize audio mixer
  Mixer mixer(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);

  Chunk sound("effect.ogg"); // OGG sound file

  // Create texture from surface containing text rendered by SDL_ttf
  Texture text(renderer, font.RenderText_Solid("Hello, world!",
               SDL_Color{255, 255, 255, 255}));

  unsigned char pixels[16 * 16 * 4];

  // Note proper constructor for Rect
  sprite1.Update(Rect(0, 0, 16, 16), pixels, 16 * 4);

  // Most setter methods are chainable
  renderer.SetLogicalSize(640, 480).SetDrawColor(0, 16, 32).Clear();

  // Also note a safe way to specify null rects and points
  renderer.Copy(sprite1, NullOpt, NullOpt);

  // There are multiple convenient ways to construct e.g. a Rect;
  // Objects provide extensive set of getters
  renderer.Copy(text, NullOpt, Rect(Point(0, 0), text.GetSize()));

  // Copy() is overloaded, providing access to both SDL_RenderCopy and SDL_RenderCopyEx
  renderer.Copy(sprite2, NullOpt, NullOpt, 45.0);

  renderer.Present();

  // Play our sound one time on a first available mixer channel
  mixer.PlayChannel(-1, sound);

  // You can still access wrapped C SDL types
//  SDL_Renderer* sdl_renderer = renderer.Get();

  // Of course, C SDL2 API is still perfectly valid
  SDL_Delay(2000);

  // All SDL objects are released at this point or if an error occurs
} catch (SDL2pp::Exception& e) {
  // Exception stores SDL_GetError() result and name of function which failed
  std::cerr << "Error in: " << e.GetSDLFunction() << std::endl;
  std::cerr << "  Reason: " << e.GetSDLError() << std::endl;
} catch (std::exception& e) {
  // This also works (e.g. "SDL_Init failed: No available video device")
  std::cerr << e.what() << std::endl;
}
return 0;
}

