#include "help_panel.h"

using namespace SEP;
help_panel:: help_panel(std::shared_ptr<position>p,std::shared_ptr<windows>my_w, 
std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, 
std::shared_ptr<pick_draw>pk, std::shared_ptr<slice_types>c,std::shared_ptr<maps>mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  

	ModesWidget=new QTextEdit("Modes"); edits.push_back(ModesWidget);
	MouseWidget=new QTextEdit("Mouse"); edits.push_back(MouseWidget);
	
	LoadWidget=new QTextEdit( "Load"); edits.push_back(LoadWidget);

	CommandWidget=new QTextEdit("CommandLine");edits.push_back(CommandWidget);

	KeysWidget=new QTextEdit("Keyboard");edits.push_back(KeysWidget);

	HistWidget=new QTextEdit("History"); edits.push_back(HistWidget);
	ClipWidget=new QTextEdit("Cliping"); edits.push_back(ClipWidget);
	createit();
	layout();

}
void help_panel::createit() {
 for(int i=0; i < (int)edits.size(); i++){
   edits[i]->setReadOnly(true);
  // edits[i]->setTextFormat(Qt:RichText) ;	
 }
 ModesWidget->append("The current mode is shown in the bottom left corner");
 ModesWidget->append("Currently there are two modes, picking and navigation.");
 ModesWidget->append("Changing modes changes the behavior of the mouse.");
 ModesWidget->append("To switch to navigation mode 'Ctrl n'.");
 ModesWidget->append("To switch to picking mode 'Ctrl p'.<br>");
 
  MouseWidget->append("<b>Navigation</b><br>");
 MouseWidget->append("Left-down begin zoom");
 MouseWidget->append("Left-release end zoom range ");
  MouseWidget->append("Left-double click unzoom dataset.");
 MouseWidget->append("Center-release move to a new location within a frame.");
 MouseWidget->append("You can also use the center button to grab the corner of a cube,");
 MouseWidget->append("cut, or three face move to change perspective.");
  MouseWidget->append("Center-release move to a new location within a frame.<br>");
 MouseWidget->append("You can also use the center button to grab the corner of a cube,");
 MouseWidget->append("cut, or three face move to change relative proportions.");
 
  MouseWidget->append("<b>Picking</b><br>");
 MouseWidget->append("Right-release add point ");
 MouseWidget->append("Center-release delete nearest point. ");

//CommandWidget->append("<b>cubeT stdin [data1=] [data2=] nviews=1] [run_history=]");
//CommandWidget->append("stdin - a regular sampled cube.");
CommandWidget->append("<b>nameless [data0=] [data2=] nviews=1] [run_history=] n1= n2= n3= [n4..n5,o1..o5,d1..d5,label1..label5] values=");
CommandWidget->append("data0- a regular sampled cube.");
CommandWidget->append("data1,data2,etc - optional additional regular cubes");
CommandWidget->append("nviews - optional - number of different view windows.");
CommandWidget->append("run_history - optional - whether or not run a set of commands contained in the specified file.");


KeysWidget->append("x,X - Move forward and backward in the dimension shown in the side view.");
KeysWidget->append("y,Y - Move forward and backward in the dimension shown in the front view.");
KeysWidget->append("z,Z - Move forward and backward in the dimension shown in the top view.");
KeysWidget->append("r -   Run a movie.");
KeysWidget->append("s -   Stop a movie");
KeysWidget->append("v -   Change perspective.");
KeysWidget->append("c -   Change colormap");


HistWidget->append("The last 10 commands are show in the bottom portion of the status window.");
HistWidget->append("By clicking on an element in the history view you can return the view to the status at that time.");
HistWidget->append("You can also save the history to a file and rerun the series of commands using the run_history command line option.");
HistWidget->append("You can add a pause within the history by adding a line of the form '0:sleep:3' which will pause the view for 3 seconds.");

ClipWidget->append("You can read float or byte data.");
ClipWidget->append("You can adjust the color mapping by changing the clip function.");
ClipWidget->append("The mapping function is show in the clip menu.");
ClipWidget->append("By default a byte value 0 is mapped to 0, 1 to 1, etc.");
ClipWidget->append("You can right click to adjust this mapping. A middle click removes the adjusment. ");
ClipWidget->append("<b>Float data</b>");
ClipWidget->append("By default float data is translated into byte data.");
ClipWidget->append("The first 5 millions samples are examined and the 99th percentile of the absolute value of the data is used to clip the data");
ClipWidget->append("You can use the command line arguments bclip (minimum clip value), eclip (maximum clip value),");
ClipWidget->append("bpclip (Lower clip based on X percentile of the data), epclip (maximum clip based on x percentile of the data),");
ClipWidget->append("pclip (clips are based on X percentile of the absolute values of the data).");
ClipWidget->append("You can also operate in float mode (float_mode=1). In this mode the entire float cube is stored in memory.");
ClipWidget->append("The same cliping parameters are used for the initial mapping.");
ClipWidget->append("If you have float data in non-native format add swap_bytes=1 to the commandline");

LoadWidget->append("You can load a new dataset using the load button in the main menu.");
LoadWidget->append("You must specify what type of data it is you wish to load, and how it will be stored in memory.");
LoadWidget->append("Currently only the SEP type datasets are supported.");

 }
void help_panel::layout() {
//	lay->addStretch(1);
	this->addTab(ModesWidget,"Modes");
	this->addTab(MouseWidget,"Mouse");
	this->addTab(LoadWidget, "Load");
	this->addTab(CommandWidget,"CommandLine");
	this->addTab(KeysWidget,"Keyboard");
	this->addTab(HistWidget,"History");
	this->addTab(ClipWidget,"Data");
					
}
