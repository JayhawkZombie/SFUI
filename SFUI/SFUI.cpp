#pragma warning ( 1 : 4265 )

#include <SFUI/Include/UI/UI.h>
#include <Windows.h>
#include <SFUI/Include/Application/AppMain.h>

// Use this function to construct the ui and add the
// constructed widgets to the application

//Return 0 to signal all elements were created successfully
int ExternCreateUI(sfui::Theme *uiTheme, std::function<void(sfui::Widget::shared_ptr)> addFunc)
{
  auto mBar = uiTheme->MakeMenuBar(sfui::AppMainWindow::AppMainWidgets.get());

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

  addFunc(mBar);

  /* Add a combobox to select the update method (parallel or sequential) */
  auto updateChoice = uiTheme->MakeComboBox();
  updateChoice->SetPosition({ 460, 25 });
  updateChoice->SetSize({ 200, 20 });
  updateChoice->SetBoxSize({ 200, 200 });
  updateChoice->AddItem("Sequential");
  updateChoice->AddItem("Parallel");
  updateChoice->SetLabel("Update Method: ");

  updateChoice->OnItemSelected(
    [ winPtr = sfui::AppMainWindow::AppMainWidgets.get() ](const std::string &ItemText)
  {
    if (ItemText == "Parallel")
      winPtr->UseParallelUpdate(true);
    else
      winPtr->UseParallelUpdate(false);
  }
  );

  addFunc(updateChoice);

  /*
  Add main tab page
  */

  auto tList = uiTheme->MakeTabList();
  tList->SetTabHeight(20);
  tList->SetDefaultSize({ sfui::WindowSize.x, 20 });
  tList->SetPanelWidth(sfui::WindowSize.x);

  auto buttonPanel = tList->AddTab("Buttons");
  auto sliderPanel = tList->AddTab("Sliders");
  auto textInputPanel = tList->AddTab("Text Inputs");
  auto textViewPanel = tList->AddTab("Text Views");
  auto listPanel = tList->AddTab("List Widgets");
  auto comboPanel = tList->AddTab("Combo Widgets");
  auto otherPanel = tList->AddTab("Other Widgets");
  tList->SetPosition({ 0, 55 });
  tList->SetPanelHeight(sfui::WindowSize.y - 55 - 20);

  addFunc(tList);

  auto tstBtn = uiTheme->MakeButton("TestButton");
  tstBtn->SetHasDropShadow(true);
  tstBtn->SetPosition({ 260, 40 });
  tstBtn->SetDefaultSize({ 150, 25 });

  auto cBox = uiTheme->MakeCheckBox("Test Check");
  cBox->SetPosition({ 10, 10 });
  cBox->SetDefaultSize({ 100, 25 });
  
  auto animBox = uiTheme->MakeComboBox();
  animBox->AddItem("Standard");
  animBox->AddItem("Accelerate");
  animBox->AddItem("Decelerate");
  animBox->AddItem("EaseInOut");
  animBox->SetSize({ 200, 25 });
  animBox->SetBoxSize({ 200, 100 });
  animBox->SetPosition({ 10, 40 });
  
  tstBtn->SetCanAnimateContract(false); tstBtn->SetCanAnimateExpand(false);
  tstBtn->OnClicked(
    [tstPtr = tstBtn.get(), aBoxPtr = animBox.get()]()
  {
    static bool moveRight = false;
    moveRight = !moveRight;
  
    sfui::Easing whichCurve = sfui::Easing::Accelerate;
    auto boxSelection = aBoxPtr->GetSelectedItem();
  
    if (boxSelection == "Standard")
      whichCurve = sfui::Easing::Standard;
    else if (boxSelection == "Accelerate")
      whichCurve = sfui::Easing::Accelerate;
    else if (boxSelection == "Decelerate")
      whichCurve = sfui::Easing::Decelerate;
    else if (boxSelection == "EaseInOut")
      whichCurve = sfui::Easing::EaseInOut;
  
    if (moveRight) {
      tstPtr->Animate(sfui::WidgetAnimation::SlideIn, tstPtr->GetPosition(), tstPtr->GetPosition() + Vec2i(500, 0), whichCurve, 750);
    }
    else {
      tstPtr->Animate(sfui::WidgetAnimation::SlideIn, tstPtr->GetPosition(), tstPtr->GetPosition() - Vec2i(500, 0), whichCurve, 750);
    }
  
  }
  );
  
  buttonPanel->Add(animBox);
  buttonPanel->Add(tstBtn);
  buttonPanel->Add(cBox);
  
  /*
    *
    *
    * Add test slider widgets
    *
    */
  auto vSlider = uiTheme->MakeValueSlider();
  vSlider->SetDefaultSize({ 15, 300 });
  vSlider->SetPosition({ 10, 10 });
  vSlider->SetBarHeight(5);
  vSlider->SetBarWidth(21);
  vSlider->SetVertical(true);  
  
  auto hSlider = uiTheme->MakeValueSlider();
  hSlider->SetDefaultSize({ 300, 15 });
  hSlider->SetPosition({ 45, 10 });
  hSlider->SetBarHeight(21);
  hSlider->SetBarWidth(5);
  hSlider->SetVertical(false);
  
  auto sBar = uiTheme->MakeScrollBar();
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
  auto lEdit = uiTheme->MakeLineEdit("opensans", 14);
  lEdit->SetPosition({ 100, 15 });
  lEdit->SetDefaultSize({ 250, 20 });
  lEdit->SetBitmapFontTracking(0);
  lEdit->SetLabel("LineEdit");
  
  auto sBox = uiTheme->MakeSpinBox();
  sBox->SetDefaultSize({ 200, 30 });
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
  
  auto lView = uiTheme->MakeListView();
  lView->SetTopWindow(sfui::AppMainWindow::AppMainWidgets.get());
  lView->SetPosition({ 40, 40 });
  lView->SetDefaultSize({ 100, 300 });
  
  for (size_t i = 0; i < 50; ++i) {
    lView->AddItem("Item " + std::to_string(i));
  }
  
  auto combo = uiTheme->MakeComboBox();
  combo->SetDefaultSize({ 200, 20 });
  combo->SetBoxSize({ 200, 300 });
  combo->SetPosition({ 150, 40 });
  combo->AddItem("Test item 1");
  combo->AddItem("Test item 2");
  combo->AddItem("Test item 3");
  
  /* TreeView */
  
  auto wTree = uiTheme->MakeTree();
  wTree->SetPosition({ 360, 40 });
  wTree->SetDefaultSize({ 200, 300 });
  
  auto root1 = wTree->NewRoot();
  root1->SetText("Root 1");
  
  auto root1button = uiTheme->MakeButton("Root 1 button");
  root1button->SetDefaultSize({ 85, 15 });
  root1->Add(root1button);
  
  auto root1btn2 = uiTheme->MakeButton("Root 1 btn2");
  root1btn2->SetDefaultSize({ 85, 15 });
  root1->Add(root1btn2);
  
  auto root2 = wTree->NewRoot();
  root2->SetText("Root 2");
  
  listPanel->Add(lView);
  listPanel->Add(combo);
  listPanel->Add(wTree);
  
  
  /*
    * Add combo widgets
    */
  auto cPicker = uiTheme->MakeColorPicker();
  cPicker->SetPosition({ 10, 10 });
  cPicker->SetDefaultSize({ 250, 250 });
  cPicker->OnColorAccepted([ ](auto color)
  {
    std::cout << "Color accepted: (" << color.r << ", " << color.g << ", " << color.b << ", " << color.a << ")\n";
  });
  
  cPicker->OnColorCancelled([ ]() { std::cout << "Color cancelled\n"; });
  comboPanel->Add(cPicker);
  
  /*
    * Add other widgets
    */
    
  auto pBar = uiTheme->MakeProgressBar();
  pBar->SetDefaultSize({ 300, 7 });
  pBar->SetPosition({ 10, 10 });

  auto progBtn = uiTheme->MakeButton("Change Progress");
  progBtn->SetPosition({ 320, 10 });
  progBtn->SetDefaultSize({ 150, 20 });
  srand(0);
  progBtn->OnClicked(
    [progPtr = pBar.get()]()
  {
    int prog = rand() % 100;
    progPtr->SetProgress(cast_float(prog));
  }
  );

  auto lSpinner = uiTheme->MakeLoadingSpinner();
  lSpinner->SetPosition({ 10, 40 });
  lSpinner->LoadTexture("../Images/WindowsLoadingHorizontal.png");
  lSpinner->SetFrameCount(37);
  lSpinner->SetSpeed(45);

  otherPanel->Add(pBar);
  otherPanel->Add(progBtn);
  otherPanel->Add(lSpinner);

  /* Add the navigation panel */
  auto navPanel = uiTheme->MakeNavPanel();
  navPanel->SetWidth(150);
  navPanel->SetClosedWidth(50);
  navPanel->SetPosition({ 0, 15 });
  navPanel->SetHeight(int(sfui::WindowSize.y));

  navPanel->AddItem("Page 1");
  navPanel->AddItem("Page 2");

  addFunc(navPanel);

  /* Add the preloader */
  auto pLoader = uiTheme->MakePreloader();
  pLoader->SetSize({ 300, 300 });
  pLoader->SetDefaultSize({ 300, 300 });
  pLoader->SetPosition({ 300, 300 });
  pLoader->SetColor(sf::Color(135, 0, 255));

  auto pStartBtn = uiTheme->MakeButton("Play Preloader");
  pStartBtn->SetPosition({ 100, 300 });
  pStartBtn->SetSize({ 130, 20 });

  pStartBtn->OnClicked(
    [pPtr = pLoader.get()]() { pPtr->Start(600, 1200, 600); }
  );

  buttonPanel->Add(pLoader);
  buttonPanel->Add(pStartBtn);

  return 0;
}
