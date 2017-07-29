#pragma warning ( 1 : 4265 )

#include <SFUI/Include/UI/UI.h>
#include <Windows.h>
#include <SFUI/Include/Application/AppMain.h>

int main(int argc, const char **argv)
{

  sfui::AppMainInitData initData;
  initData.argc = argc;
  initData.argv = argv;
  initData.WindowTitle = "SFUI";
  initData.WindowSize = Vec2i(1400, 800);
  initData.WindowStyle = sf::Style::Default;

  sfui::AppMainWindow::Init(initData);

  texture_handle iconTexture = std::make_shared<sf::Texture>();
  iconTexture->loadFromFile("iconmap_grey.png");

  font_handle hFont = std::make_shared<sf::Font>();
  if (!hFont->loadFromFile("segoeui.ttf")) {
    std::cerr << "Unable to load font" << std::endl;
    return -1;
  }

  sfui::Theme::shared_ptr theme = sfui::Theme::Create();
  theme->DefaultFont = hFont;
  theme->IconTexture = iconTexture;

  sfui::AppMainWindow::SetDefaultTheme(theme);

  /*
    FONT SMOOTHING + CLEAR TYPE 

    DO NOT RENDER FROM TRUETYPE OUTLINE AND DO NOT USE TRUETYPE HINTING
    DO NOT USE SUPERSAMPLING
  
  */
  //theme->BitmapFonts.LoadFont("arial", "Fonts/BitmapFonts/Images/arial14.png", "Fonts/BitmapFonts/Data/arial14.fnt", 14);
  //theme->BitmapFonts.LoadFont("arial", "Fonts/BitmapFonts/Images/arial18.png", "Fonts/BitmapFonts/Data/arial18.fnt", 18);
  //theme->BitmapFonts.LoadFont("arial", "Fonts/BitmapFonts/Images/arial24.png", "Fonts/BitmapFonts/Data/arial24.fnt", 24);

  //theme->BitmapFonts.LoadFont("segoe", "Fonts/BitmapFonts/Images/sugo14.png", "Fonts/BitmapFonts/Data/sugo14.fnt", 14);
  //theme->BitmapFonts.LoadFont("segoe", "Fonts/BitmapFonts/Images/sugo18.png", "Fonts/BitmapFonts/Data/sugo18.fnt", 18);
  //theme->BitmapFonts.LoadFont("segoe", "Fonts/BitmapFonts/Images/sugo22.png", "Fonts/BitmapFonts/Data/sugo22.fnt", 22);

  //theme->BitmapFonts.LoadFont("segoesl", "Fonts/BitmapFonts/Images/sugosl14.png", "Fonts/BitmapFonts/Data/sugosl14.fnt", 14);
  //theme->BitmapFonts.LoadFont("segoesl", "Fonts/BitmapFonts/Images/sugosl18.png", "Fonts/BitmapFonts/Data/sugosl18.fnt", 18);

  //theme->BitmapFonts.LoadFont("segoesl_nosmooth", "Fonts/BitmapFonts/Images/sugosl14_nosmooth.png", "Fonts/BitmapFonts/Data/sugosl14_nosmooth.fnt", 14);

  //theme->BitmapFonts.LoadFont("verdana", "Fonts/BitmapFonts/Images/verdana18.png", "Fonts/BitmapFonts/Data/verdana18.fnt", 18);
  theme->BitmapFonts.LoadFont("opensans", "Fonts/BitmapFonts/Images/osans14.png", "Fonts/BitmapFonts/Data/osans14.fnt", 14);
  theme->BitmapFonts.LoadFont("sugo", "Fonts/BitmapFonts/Images/sugo16_light.png", "Fonts/BitmapFonts/Data/sugo16_light.fnt", 16);

  //theme->BitmapFonts.LoadFont("calibri", "Fonts/BitmapFonts/Images/cali14.png", "Fonts/BitmapFonts/Data/cali14.fnt", 14);
  //theme->BitmapFonts.LoadFont("calibri", "Fonts/BitmapFonts/Images/cali18.png", "Fonts/BitmapFonts/Data/cali18.fnt", 18);
  //theme->BitmapFonts.LoadFont("calibri", "Fonts/BitmapFonts/Images/cali24.png", "Fonts/BitmapFonts/Data/cali24.fnt", 24);

  theme->DefaultBitmapFont = theme->BitmapFonts.GetFont("sugo", 16).value_or(nullptr);

  /************************************************************************/
  /* Add MenuBar                                                          */
  /************************************************************************/
  auto mBar = theme->MakeMenuBar(sfui::AppMainWindow::AppMainWidgets.get());

  auto fileMenu = mBar->AddMenu("File");
  fileMenu->Add("Open");
  fileMenu->Add("Save");
  fileMenu->Add("Exit");

  mBar->OnMenuItemSelected([ ](auto menuText, auto itemText) {
    if (menuText == "File" && itemText == "Exit") {
      std::cout << "Trying to launch confirmation box\n";
      auto handle = sfui::AppMainWindow::Confirm({ 400, 400 }, "Confirm", sf::Style::Default,
                                                 "Please Confirm", "No", "Yes",
                                                 [ ]() {std::cout << "Cancelled\n"; }, [ ]() {std::cout << "Accepted\n"; });
      handle->Open();
    }
  });

  auto editMenu = mBar->AddMenu("Edit");
  editMenu->Add("Undo");
  editMenu->Add("-----------");
  editMenu->Add("Settings");

  auto helpMenu = mBar->AddMenu("Help");
  helpMenu->Add("Report a bug");
  helpMenu->Add("Contact us");
  helpMenu->Add("About");
  sfui::AppMainWindow::AddWidget(mBar);

  /*
    Add main tab page
  */

  auto tList = theme->MakeTabList();
  tList->SetTabHeight(20);
  tList->SetSize({ initData.WindowSize.x, 20 });
  tList->SetPanelWidth(initData.WindowSize.x);

  auto buttonPanel = tList->AddTab("Buttons");
  auto sliderPanel = tList->AddTab("Sliders");
  auto textInputPanel = tList->AddTab("Text Inputs");
  auto textViewPanel = tList->AddTab("Text Views");
  auto listPanel = tList->AddTab("List Widgets");
  auto comboPanel = tList->AddTab("Combo Widgets");
  auto otherPanel = tList->AddTab("Other Widgets");
  tList->SetPosition({ 0, 55 });
  tList->SetPanelHeight(initData.WindowSize.y - 55 - 20);

  /**
    *
    * Add test button widgets
    *
    */

  sfui::AppMainWindow::AddWidget(tList);
  auto tstBtn = theme->MakeButton("TestButton");
  tstBtn->SetPosition({ 10, 20 });
  tstBtn->SetSize({ 150, 25 });

  auto cBox = theme->MakeCheckBox("Test Check");
  cBox->SetPosition({ 10, 90 });
  cBox->SetSize({ 100, 25 });

  buttonPanel->Add(tstBtn);
  buttonPanel->Add(cBox);

  /*
    *
    *
    * Add test slider widgets
    *
    */
  auto vSlider = theme->MakeValueSlider();
  vSlider->SetSize({ 15, 300 });
  vSlider->SetPosition({ 10, 10 });
  vSlider->SetBarHeight(5);
  vSlider->SetBarWidth(21);
  vSlider->SetVertical(true);


  auto hSlider = theme->MakeValueSlider();
  hSlider->SetSize({ 300, 15 });
  hSlider->SetPosition({ 45, 10 });
  hSlider->SetBarHeight(21);
  hSlider->SetBarWidth(5);
  hSlider->SetVertical(false);

  auto sBar = theme->MakeScrollBar();
  sBar->SetIsVertical(true);
  sBar->SetSize({ 17, 400 });
  sBar->SetPosition({ 50, 50 });
  sBar->SetRange(500, 800, 1300);

  sliderPanel->Add(vSlider);
  sliderPanel->Add(hSlider);
  sliderPanel->Add(sBar);


  /*
   * Add test text input widgets
   *
   **/
  auto lEdit = theme->MakeLineEdit("opensans", 14);
  lEdit->SetPosition({ 100, 15 });
  lEdit->SetSize({ 250, 20 });
  lEdit->SetBitmapFontTracking(0);
  lEdit->SetLabel("LineEdit");

  auto sBox = theme->MakeSpinBox();
  sBox->SetSize({ 200, 30 });
  sBox->SetPosition({ 10, 40 });
  sBox->SetIsIntegral(false);

  sBox->SetDefaultValue(0.f);
  sBox->SetValueIncrease(1.f);
  sBox->SetValueDecrease(1.f);
  sBox->SetMaximumValue(100.f);
  sBox->SetMinimumValue(-100.f);

  textInputPanel->Add(lEdit);
  textInputPanel->Add(sBox);

  /*
  *
  * Add test text views
  */
  auto mlView = sfui::MultilineTextView::Create();
  mlView->LoadFontTexture("Fonts/BitmapFonts/Images/osans14_ns.png");
  mlView->LoadFontData("Fonts/BitmapFonts/Data/osans14_ns.fnt");
  mlView->SetPosition({ 10, 10 });
  mlView->SetColor(sf::Color::White);
  mlView->SetText("This is a simple test string that has no line breaks and lots of short words");
  textViewPanel->Add(mlView);

  /*
   * Add test list widgets
   *
   **/

  auto lView = theme->MakeListView();
  lView->SetTopWindow(sfui::AppMainWindow::AppMainWidgets.get());
  lView->SetPosition({ 40, 40 });
  lView->SetSize({ 100, 300 });

  for (size_t i = 0; i < 50; ++i) {
    lView->AddItem("Item " + std::to_string(i));
  }

  auto combo = theme->MakeComboBox();
  combo->SetSize({ 200, 20 });
  combo->SetBoxSize({ 200, 300 });
  combo->SetPosition({ 150, 40 });
  combo->AddItem("Test item 1");
  combo->AddItem("Test item 2");
  combo->AddItem("Test item 3");

  /* TreeView */

  auto wTree = theme->MakeTree();
  wTree->SetPosition({ 360, 40 });
  wTree->SetSize({ 200, 300 });

  auto root1 = wTree->NewRoot();
  root1->SetText("Root 1");

  auto root1button = theme->MakeButton("Root 1 button");
  root1button->SetSize({ 85, 15 });
  root1->Add(root1button);

  auto root1btn2 = theme->MakeButton("Root 1 btn2");
  root1btn2->SetSize({ 85, 15 });
  root1->Add(root1btn2);

  auto root2 = wTree->NewRoot();
  root2->SetText("Root 2");

  listPanel->Add(lView);
  listPanel->Add(combo);
  listPanel->Add(wTree);


  /*
   * Add combo widgets
   */
  auto cPicker = theme->MakeColorPicker();
  cPicker->SetPosition({ 10, 10 });
  cPicker->SetSize({ 250, 250 });
  cPicker->OnColorAccepted([ ](auto color)
  {
    std::cout << "Color accepted: (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")\n";
  });

  cPicker->OnColorCancelled([ ]() { std::cout << "Color cancelled\n"; });
  comboPanel->Add(cPicker);

  /*
   * Add other widgets
   */
  
   auto pBar = theme->MakeProgressBar();
   pBar->SetSize({ 300, 7 });
   pBar->SetPosition({ 10, 10 });
   
   auto progBtn = theme->MakeButton("Change Progress");
   progBtn->SetPosition({ 320, 10 });
   progBtn->SetSize({ 150, 20 });
   srand(0);
   progBtn->OnClicked(
     [progPtr = pBar.get()]()
   {
     int prog = rand() % 100;
     progPtr->SetProgress(cast_float(prog));
   }
   );

   auto lSpinner = theme->MakeLoadingSpinner();
   lSpinner->SetPosition({ 10, 40 });
   lSpinner->LoadTexture("../Images/WindowsLoadingHorizontal.png");
   lSpinner->SetFrameCount(37);
   lSpinner->SetSpeed(45);

   otherPanel->Add(pBar);
   otherPanel->Add(progBtn);
   otherPanel->Add(lSpinner);

  int evntRet = 0;
  int updRet = 0;
  int rndRet = 0;

  while (sfui::AppMainWindow::IsRunning()) {

    evntRet = sfui::AppMainWindow::ProcessEvents();

    updRet = sfui::AppMainWindow::Update();

    rndRet = sfui::AppMainWindow::Render();

  }

  sfui::AppMainWindow::Shutdown();

  return 0;
}

