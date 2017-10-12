#include "ano_panel.h"

using namespace SEP;
ano_panel:: ano_panel(std::shared_ptr<position> p,std::shared_ptr<windows>my_w, 
 std::shared_ptr<panels>my_p, std::shared_ptr<datasets>my_d, std::shared_ptr<pick_draw> pk, 
 std::shared_ptr<slice_types> c,std::shared_ptr<maps> mym){
  set_basics(p,my_w,my_p,my_d,pk,c,mym);
  
  std::shared_ptr<QHBoxLayout> lt(new QHBoxLayout()),lb(new QHBoxLayout());
  std::shared_ptr<QVBoxLayout>  lm(new QVBoxLayout());
  std::shared_ptr<QGroupBox> gb(new QGroupBox()),gb2(new QGroupBox());
       layTop=lt;

     
     layBot=lb;
     layMain=lm;
     gbox=gb;
          gbox2=gb2;

     shape=new basicRadioBox("Shape","box","Draw a box");
     shape->addButton("text","Draw text");
     shape->addButton("arrow","Draw an arrow");
     shape->addButton("line","Draw a line");
     shape->addButton("elipse","Draw an elipse");    
     shape->set_checked(0);
     std::vector<QString> len;
     for(int i=0; i < 20; i++) len.push_back(QString::number(i));
     std::shared_ptr<basicComboBox> th(new basicComboBox("Thickness","Thickness of line",len));
     thick=th;
     thick->set_current(0,3);
     std::vector<QString> color=my_c->return_cvec();
     std::shared_ptr<basicComboBox> cb(new basicComboBox("Color","Color of line/text",color));
     colorB=cb;
     std::shared_ptr<basicComboBox> fn(new basicComboBox("Font","Font to use for text",my_f->font_list));
    font=fn;
    
    txt=new basicLineEditBox("Text",20,"Enter","Text to annotate with");


  	connect(shape->my_radios[0], SIGNAL(clicked()), this, SLOT(sbox_change()));
  	connect(shape->my_radios[1], SIGNAL(clicked()), this, SLOT(stext_change()));
  	connect(shape->my_radios[2], SIGNAL(clicked()), this, SLOT(sarrow_change()));
  	connect(shape->my_radios[3], SIGNAL(clicked()), this, SLOT(sline_change()));
  	connect(shape->my_radios[4], SIGNAL(clicked()), this, SLOT(selipse_change()));
    connect(colorB->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(color_change(const QString&)));
    connect(thick->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(thick_change(const QString&)));
    connect(font->my_combos[0], SIGNAL(activated(const QString&)),this, SLOT(font_change(const QString&)));
    connect(txt->my_edits[0], SIGNAL(textChanged(const QString&)),this, SLOT(text_change(const QString&)));


    
    layBot->addWidget(thick->group());    
    layBot->addWidget(font->group());
    layBot->addWidget(txt->group());
    
    gbox->setLayout(layBot.get());
    layTop->addWidget(shape->group());
        layTop->addWidget(colorB->group());

    gbox2->setLayout(layTop.get());
    layMain->addWidget(gbox2.get() );
    layMain->addWidget(gbox.get());
    setLayout(layMain.get());
  

}
void ano_panel::update_menu(std::vector<QString>){


}
void ano_panel::delete_ano_panel(){
  delete shape;
  delete txt;
 
}
void ano_panel::sbox_change(){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("shape");
   coms.push_back("box");
   emit actionDetected(coms);
}
void ano_panel::stext_change(){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("shape");
   coms.push_back("text");
   emit actionDetected(coms);
}
void ano_panel::sarrow_change(){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("shape");
   coms.push_back("arrow");
   emit actionDetected(coms);
}
void ano_panel::sline_change(){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("shape");
   coms.push_back("line");
   emit actionDetected(coms);
}
void ano_panel::selipse_change(){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("shape");
   coms.push_back("elipse");
   emit actionDetected(coms);
}
void ano_panel::color_change(const QString c){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("color");
   coms.push_back(c);
   emit actionDetected(coms);
}

void ano_panel::thick_change(const QString c){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("thick");
   coms.push_back(c);
   emit actionDetected(coms);
}
void ano_panel::text_change(const QString c){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("text");
   coms.push_back(c);
   emit actionDetected(coms);
}


void ano_panel::font_change(const QString c){
   std::vector<QString> coms;
   coms.push_back("annotate");
   coms.push_back("font");
   coms.push_back(c);
   emit actionDetected(coms);
}



