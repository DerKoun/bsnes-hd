struct Settings : Markup::Node {
  using string = nall::string;

  auto load() -> void;
  auto save() -> void;
  auto process(bool load) -> void;

  string location;

  struct Video {
    string driver;
    bool exclusive = false;
    bool blocking = false;
    bool flush = false;
    string format = "Default";
    string shader = "Blur";

    uint luminance = 100;
    uint saturation = 100;
    uint gamma = 150;

    string output = "Scale";
    uint multiplier = 2;
    bool aspectCorrection = true;
    bool overscan = false;
    bool blur = false;
  } video;

  struct Audio {
    string driver;
    bool exclusive = false;
    string device;
    bool blocking = true;
    bool dynamic = false;
    uint frequency = 48000;
    uint latency = 0;

    bool mute = false;
    int skew = 0;
    uint volume = 100;
    uint balance = 50;
  } audio;

  struct Input {
    string driver;
    uint frequency = 5;
    string defocus = "Pause";
    struct Turbo {
      uint frequency = 4;
    } turbo;
  } input;

  struct Path {
    string games;
    string patches;
    string saves;
    string cheats;
    string states;
    string screenshots;
    struct Recent {
      string superFamicom;
      string gameBoy;
      string bsMemory;
      string sufamiTurboA;
      string sufamiTurboB;
    } recent;
  } path;

  struct Emulator {
    bool warnOnUnverifiedGames = false;
    struct AutoSaveMemory {
      bool enable = true;
      uint interval = 30;
    } autoSaveMemory;
    bool autoSaveStateOnUnload = false;
    bool autoLoadStateOnLoad = false;
    struct Hack {
      struct PPU {
        bool fast = true;
        bool noSpriteLimit = true;
        struct Mode7 {
          uint scale = 2;
          uint perspective = 1;
          uint supersample = 1;
          uint mosaic = 1;
          uint widescreen = 72;
          uint wsbg1 = 16;
          uint wsbg2 = 16;
          uint wsbg3 = 16;
          uint wsbg4 = 16;
          uint wsobj = 0;
          uint igwin = 1;
          uint igwinx = 128;
          uint bgGrad = 4;
		  uint windRad = 0;
          uint wsMode = 1;
          uint wsBgCol = 1;
          uint unintrMode = 1;
          uint unintrTop = 10;
          uint unintrBottom = 10;
          uint unintrLeft = 20;
          uint unintrRight = 20;
          uint wsMarker = 0;
          uint wsMarkerAlpha = 1;
        } mode7;
      } ppu;
      struct DSP {
        bool fast = true;
        bool cubic = false;
      } dsp;
      struct Coprocessors {
        bool delayedSync = true;
        bool hle = true;
      } coprocessors;
      uint fastSuperFX = 100;
    } hack;
    struct Cheats {
      bool enable = true;
    } cheats;
  } emulator;

  struct General {
    bool statusBar = true;
    bool screenSaver = false;
    bool toolTips = true;
    bool crashed = false;
  } general;
};

struct VideoSettings : TabFrameItem {
  auto create() -> void;

private:
  VerticalLayout layout{this};
    Label colorAdjustmentLabel{&layout, Size{~0, 0}, 2};
    TableLayout colorLayout{&layout, Size{~0, 0}};
      Label luminanceLabel{&colorLayout, Size{0, 0}};
      Label luminanceValue{&colorLayout, Size{50_sx, 0}};
      HorizontalSlider luminanceSlider{&colorLayout, Size{~0, 0}};
    //
      Label saturationLabel{&colorLayout, Size{0, 0}};
      Label saturationValue{&colorLayout, Size{50_sx, 0}};
      HorizontalSlider saturationSlider{&colorLayout, Size{~0, 0}};
    //
      Label gammaLabel{&colorLayout, Size{0, 0}};
      Label gammaValue{&colorLayout, Size{50_sx, 0}};
      HorizontalSlider gammaSlider{&colorLayout, Size{~0, 0}};
};

struct AudioSettings : TabFrameItem {
  auto create() -> void;

private:
  VerticalLayout layout{this};
    Label effectsLabel{&layout, Size{~0, 0}, 2};
    TableLayout effectsLayout{&layout, Size{~0, 0}};
      Label skewLabel{&effectsLayout, Size{0, 0}};
      Label skewValue{&effectsLayout, Size{50_sx, 0}};
      HorizontalSlider skewSlider{&effectsLayout, Size{~0, 0}};
    //
      Label volumeLabel{&effectsLayout, Size{0, 0}};
      Label volumeValue{&effectsLayout, Size{50_sx, 0}};
      HorizontalSlider volumeSlider{&effectsLayout, Size{~0, 0}};
    //
      Label balanceLabel{&effectsLayout, Size{0, 0}};
      Label balanceValue{&effectsLayout, Size{50_sx, 0}};
      HorizontalSlider balanceSlider{&effectsLayout, Size{~0, 0}};
};

struct InputSettings : TabFrameItem {
  auto create() -> void;
  auto updateControls() -> void;
  auto activePort() -> InputPort&;
  auto activeDevice() -> InputDevice&;
  auto reloadPorts() -> void;
  auto reloadDevices() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto cancelMapping() -> void;
  auto assignMouseInput(uint id) -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue, bool allowMouseInput = false) -> void;

  maybe<InputMapping&> activeMapping;

private:
  Timer timer;

  VerticalLayout layout{this};
    HorizontalLayout selectionLayout{&layout, Size{~0, 0}};
      Label portLabel{&selectionLayout, Size{0, 0}};
      ComboButton portList{&selectionLayout, Size{~0, 0}};
      Label deviceLabel{&selectionLayout, Size{0, 0}};
      ComboButton deviceList{&selectionLayout, Size{~0, 0}};
      Label turboLabel{&selectionLayout, Size{0, 0}};
      ComboButton turboList{&selectionLayout, Size{0, 0}};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button assignMouse1{&controlLayout, Size{100_sx, 0}};
      Button assignMouse2{&controlLayout, Size{100_sx, 0}};
      Button assignMouse3{&controlLayout, Size{100_sx, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button assignButton{&controlLayout, Size{80_sx, 0}};
      Button clearButton{&controlLayout, Size{80_sx, 0}};
};

struct HotkeySettings : TabFrameItem {
  auto create() -> void;
  auto reloadMappings() -> void;
  auto refreshMappings() -> void;
  auto assignMapping() -> void;
  auto cancelMapping() -> void;
  auto inputEvent(shared_pointer<HID::Device> device, uint group, uint input, int16 oldValue, int16 newValue) -> void;

  maybe<InputMapping&> activeMapping;

private:
  Timer timer;

  VerticalLayout layout{this};
    TableView mappingList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button assignButton{&controlLayout, Size{80_sx, 0}};
      Button clearButton{&controlLayout, Size{80_sx, 0}};
};

struct PathSettings : TabFrameItem {
  auto create() -> void;
  auto refreshPaths() -> void;

public:
  TableLayout layout{this};
    Label gamesLabel{&layout, Size{0, 0}};
    LineEdit gamesPath{&layout, Size{~0, 0}};
    Button gamesAssign{&layout, Size{80_sx, 0}};
    Button gamesReset{&layout, Size{80_sx, 0}};
  //
    Label patchesLabel{&layout, Size{0, 0}};
    LineEdit patchesPath{&layout, Size{~0, 0}};
    Button patchesAssign{&layout, Size{80_sx, 0}};
    Button patchesReset{&layout, Size{80_sx, 0}};
  //
    Label savesLabel{&layout, Size{0, 0}};
    LineEdit savesPath{&layout, Size{~0, 0}};
    Button savesAssign{&layout, Size{80_sx, 0}};
    Button savesReset{&layout, Size{80_sx, 0}};
  //
    Label cheatsLabel{&layout, Size{0, 0}};
    LineEdit cheatsPath{&layout, Size{~0, 0}};
    Button cheatsAssign{&layout, Size{80_sx, 0}};
    Button cheatsReset{&layout, Size{80_sx, 0}};
  //
    Label statesLabel{&layout, Size{0, 0}};
    LineEdit statesPath{&layout, Size{~0, 0}};
    Button statesAssign{&layout, Size{80_sx, 0}};
    Button statesReset{&layout, Size{80_sx, 0}};
  //
    Label screenshotsLabel{&layout, Size{0, 0}};
    LineEdit screenshotsPath{&layout, Size{~0, 0}};
    Button screenshotsAssign{&layout, Size{80_sx, 0}};
    Button screenshotsReset{&layout, Size{80_sx, 0}};
};

struct EmulatorSettings : TabFrameItem {
  auto create() -> void;
  auto updateConfiguration() -> void;

public:
  VerticalLayout layout{this};
    Label hacksNote{&layout, Size{~0, 0}};
    Label optionsLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout inputFocusLayout{&layout, Size{~0, 0}};
      Label inputFocusLabel{&inputFocusLayout, Size{0, 0}};
      RadioLabel pauseEmulation{&inputFocusLayout, Size{0, 0}};
      RadioLabel blockInput{&inputFocusLayout, Size{0, 0}};
      RadioLabel allowInput{&inputFocusLayout, Size{0, 0}};
      Group inputFocusGroup{&pauseEmulation, &blockInput, &allowInput};
    CheckLabel warnOnUnverifiedGames{&layout, Size{~0, 0}};
    HorizontalLayout autoStateLayout{&layout, Size{~0, 0}};
      CheckLabel autoSaveMemory{&autoStateLayout, Size{~0, 0}};
      CheckLabel autoSaveStateOnUnload{&autoStateLayout, Size{0, 0}};
      CheckLabel autoLoadStateOnLoad{&autoStateLayout, Size{0, 0}};
    Label coprocessorLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout coprocessorsLayout{&layout, Size{~0, 0}};
      Label superFXLabel{&coprocessorsLayout, Size{0, 0}};
      Label superFXValue{&coprocessorsLayout, Size{50_sx, 0}};
      HorizontalSlider superFXClock{&coprocessorsLayout, Size{~0, 0}};
      CheckLabel coprocessorsDelayedSyncOption{&coprocessorsLayout, Size{0, 0}};
      CheckLabel coprocessorsHLEOption{&coprocessorsLayout, Size{0, 0}};
    Label ppuDspLabel{&layout, Size{~0, 0}, 2};
    HorizontalLayout ppuDspLayout{&layout, Size{~0, 0}};
      CheckLabel fastPPU{&ppuDspLayout, Size{0, 0}};
      CheckLabel noSpriteLimit{&ppuDspLayout, Size{0, 0}};
      CheckLabel fastDSP{&ppuDspLayout, Size{0, 0}};
      CheckLabel cubicInterpolation{&ppuDspLayout, Size{0, 0}};
    Label mode7Label{&layout, Size{~0, 0}, 2};
    HorizontalLayout hdALayout{&layout, Size{~0, 0}};
      Label mode7ScaleLabel{&hdALayout, Size{0, 0}};
      ComboButton mode7Scale{&hdALayout, Size{0, 0}};
      Label mode7PerspectiveLabel{&hdALayout, Size{0, 0}};
      ComboButton mode7Perspective{&hdALayout, Size{0, 0}};
      Label mode7MosaicLabel{&hdALayout, Size{0, 0}};
      ComboButton mode7Mosaic{&hdALayout, Size{0, 0}};
      Label mode7SupersampleLabel{&hdALayout, Size{0, 0}};
      ComboButton mode7Supersample{&hdALayout, Size{0, 0}};
    HorizontalLayout hdBLayout{&layout, Size{~0, 0}};
      Label wsModeLabel{&hdBLayout, Size{0, 0}};
      ComboButton wsMode{&hdBLayout, Size{0, 0}};
      Label mode7WidescreenLabel{&hdBLayout, Size{0, 0}};
      ComboButton mode7Widescreen{&hdBLayout, Size{0, 0}};
      Label wsBgColLabel{&hdBLayout, Size{0, 0}};
      ComboButton wsBgCol{&hdBLayout, Size{0, 0}};
      Label wsMarkerLabel{&hdBLayout, Size{0, 0}};
      ComboButton wsMarker{&hdBLayout, Size{0, 0}};
      Label wsMarkerAlphaLabel{&hdBLayout, Size{0, 0}};
      ComboButton wsMarkerAlpha{&hdBLayout, Size{0, 0}};
    HorizontalLayout hdCLayout{&layout, Size{~0, 0}};
      Label wsBG1Label{&hdCLayout, Size{0, 0}};
      ComboButton wsBG1{&hdCLayout, Size{0, 0}};
      Label wsBG2Label{&hdCLayout, Size{0, 0}};
      ComboButton wsBG2{&hdCLayout, Size{0, 0}};
      Label wsBG3Label{&hdCLayout, Size{0, 0}};
      ComboButton wsBG3{&hdCLayout, Size{0, 0}};
      Label wsBG4Label{&hdCLayout, Size{0, 0}};
      ComboButton wsBG4{&hdCLayout, Size{0, 0}};
      Label wsObjLabel{&hdCLayout, Size{0, 0}};
      ComboButton wsObj{&hdCLayout, Size{0, 0}};
    HorizontalLayout hdDLayout{&layout, Size{~0, 0}};
      Label igwinLabel{&hdDLayout, Size{0, 0}};
      ComboButton igwin{&hdDLayout, Size{0, 0}};
      Label igwinxLabel{&hdDLayout, Size{0, 0}};
      ComboButton igwinx{&hdDLayout, Size{0, 0}};
      Label bgGradLabel{&hdDLayout, Size{0, 0}};
      ComboButton bgGrad{&hdDLayout, Size{0, 0}};
      Label windRadLabel{&hdDLayout, Size{0, 0}};
      ComboButton windRad{&hdDLayout, Size{0, 0}};
    HorizontalLayout hdELayout{&layout, Size{~0, 0}};
      Label unintrModeLabel{&hdELayout, Size{0, 0}};
      ComboButton unintrMode{&hdELayout, Size{0, 0}};
      Label unintrTopLabel{&hdELayout, Size{0, 0}};
      ComboButton unintrTop{&hdELayout, Size{0, 0}};
      Label unintrBottomLabel{&hdELayout, Size{0, 0}};
      ComboButton unintrBottom{&hdELayout, Size{0, 0}};
      Label unintrLeftLabel{&hdELayout, Size{0, 0}};
      ComboButton unintrLeft{&hdELayout, Size{0, 0}};
      Label unintrRightLabel{&hdELayout, Size{0, 0}};
      ComboButton unintrRight{&hdELayout, Size{0, 0}};
};

struct DriverSettings : TabFrameItem {
  auto create() -> void;
  auto videoDriverChanged() -> void;
  auto videoDriverChange() -> void;
  auto videoFormatChanged() -> void;
  auto videoFormatChange() -> void;
  auto audioDriverChanged() -> void;
  auto audioDriverChange() -> void;
  auto audioDeviceChanged() -> void;
  auto audioDeviceChange() -> void;
  auto audioFrequencyChanged() -> void;
  auto audioFrequencyChange() -> void;
  auto audioLatencyChanged() -> void;
  auto audioLatencyChange() -> void;
  auto inputDriverChanged() -> void;
  auto inputDriverChange() -> void;

public:
  VerticalLayout layout{this};
    Label videoLabel{&layout, Size{~0, 0}, 2};
    TableLayout videoLayout{&layout, Size{~0, 0}};
      Label videoDriverLabel{&videoLayout, Size{0, 0}};
      HorizontalLayout videoDriverLayout{&videoLayout, Size{~0, 0}};
        ComboButton videoDriverOption{&videoDriverLayout, Size{0, 0}};
        Button videoDriverUpdate{&videoDriverLayout, Size{0, 0}};
        Label videoDriverActive{&videoDriverLayout, Size{0, 0}};
      Label videoFormatLabel{&videoLayout, Size{0, 0}};
      HorizontalLayout videoPropertyLayout{&videoLayout, Size{~0, 0}};
        ComboButton videoFormatOption{&videoPropertyLayout, Size{0, 0}};
    HorizontalLayout videoToggleLayout{&layout, Size{~0, 0}};
      CheckLabel videoExclusiveToggle{&videoToggleLayout, Size{0, 0}};
      CheckLabel videoBlockingToggle{&videoToggleLayout, Size{0, 0}};
      CheckLabel videoFlushToggle{&videoToggleLayout, Size{0, 0}};
    Canvas videoSpacer{&layout, Size{~0, 1}};
    Label audioLabel{&layout, Size{~0, 0}, 2};
    TableLayout audioLayout{&layout, Size{~0, 0}};
      Label audioDriverLabel{&audioLayout, Size{0, 0}};
      HorizontalLayout audioDriverLayout{&audioLayout, Size{~0, 0}};
        ComboButton audioDriverOption{&audioDriverLayout, Size{0, 0}};
        Button audioDriverUpdate{&audioDriverLayout, Size{0, 0}};
        Label audioDriverActive{&audioDriverLayout, Size{0, 0}};
      Label audioDeviceLabel{&audioLayout, Size{0, 0}};
      HorizontalLayout audioPropertyLayout{&audioLayout, Size{~0, 0}};
        ComboButton audioDeviceOption{&audioPropertyLayout, Size{0, 0}};
        Label audioFrequencyLabel{&audioPropertyLayout, Size{0, 0}};
        ComboButton audioFrequencyOption{&audioPropertyLayout, Size{0, 0}};
        Label audioLatencyLabel{&audioPropertyLayout, Size{0, 0}};
        ComboButton audioLatencyOption{&audioPropertyLayout, Size{0, 0}};
    HorizontalLayout audioToggleLayout{&layout, Size{~0, 0}};
      CheckLabel audioExclusiveToggle{&audioToggleLayout, Size{0, 0}};
      CheckLabel audioBlockingToggle{&audioToggleLayout, Size{0, 0}};
      CheckLabel audioDynamicToggle{&audioToggleLayout, Size{0, 0}};
    Canvas audioSpacer{&layout, Size{~0, 1}};
    Label inputLabel{&layout, Size{~0, 0}, 2};
    TableLayout inputLayout{&layout, Size{~0, 0}};
      Label inputDriverLabel{&inputLayout, Size{0, 0}};
      HorizontalLayout inputDriverLayout{&inputLayout, Size{~0, 0}};
        ComboButton inputDriverOption{&inputDriverLayout, Size{0, 0}};
        Button inputDriverUpdate{&inputDriverLayout, Size{0, 0}};
        Label inputDriverActive{&inputDriverLayout, Size{0, 0}};
};

struct SettingsWindow : Window {
  auto create() -> void;
  auto setVisible(bool visible = true) -> SettingsWindow&;
  auto show(uint index) -> void;

public:
  VerticalLayout layout{this};
    TabFrame panel{&layout, Size{~0, ~0}};
  StatusBar statusBar{this};
};

extern Settings settings;
extern VideoSettings videoSettings;
extern AudioSettings audioSettings;
extern InputSettings inputSettings;
extern HotkeySettings hotkeySettings;
extern PathSettings pathSettings;
extern EmulatorSettings emulatorSettings;
extern DriverSettings driverSettings;
extern SettingsWindow settingsWindow;
