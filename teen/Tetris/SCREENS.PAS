program screenTest;
{$M 22000,0,655360}
uses crt, graph;

var graphDriver : integer;
    graphMode : integer;


procedure drawScoreScreen;
begin { drawScoreScreen }
 clearDevice;
 setFillStyle(ltBkSlashFill,1);
 bar(1,1,320,199);
 setLineStyle(solidLn,0,thickWidth);
 setFillStyle(emptyFill,0);
 setColor(3);
 bar(10,15,90,50);
 bar(100,15,310,170);
 setColor(2);
 setTextStyle(triplexFont,horizDir,3);
 outTextXY(14,10,'TETRIS');
 line(14,36,86,36);
 setTextStyle(defaultFont,horizDir,1);
 outTextXY(15,41,'Lumpy ''95');
end;  { drawScoreScreen }

procedure button(X1,Y1,X2,Y2 : integer);
begin { button }
 setFillStyle(solidFill,1); bar(X1-1,Y1-1,X2+1,Y2+1);
 setLineStyle(solidLn,0,normWidth);
 setColor(0); rectangle(X1,Y1,X2,Y2);
              rectangle(X1+1,Y1+1,X2-1,Y2-1);
 setColor(3); line(X1,Y2,X1,Y1); line(X1,Y1,X2,Y1);
              line(X1+1,Y2-1,X1+1,Y1+1); line(X1+1,Y1+1,X2-1,Y1+1);
 setFillStyle(solidFill,2); bar(X1+2,Y1+2,X2-2,Y2-2);
end;  { button }

procedure draw1PlayerScreen;
begin { draw1PlayerScreen };
 clearDevice;
 setFillStyle(ltBkSlashFill,1);
 bar(1,1,320,199);
 setLineStyle(solidLn,0,thickWidth);
 setFillStyle(emptyFill,0);
 setColor(3);
 bar(106,1,213,198);
 rectangle(104,1,215,198);
 rectangle(102,1,217,198);
 bar(245,20,300,75);
 bar(245,105,300,183);
 bar(10,15,90,50);
 bar(10,93,91,180);

 setColor(2);
 setTextStyle(triplexFont,horizDir,3);
 outTextXY(14,10,'TETRIS');
 line(14,36,86,36);
 setTextStyle(defaultFont,horizDir,1);
 outTextXY(15,41,'Lumpy ''95');

 button(20,100,35,114);
 button(20,120,40,134);
 button(45,120,60,134);
 button(20,140,35,154);
 button(20,160,35,174);
 setColor(0);
 outTextXY(50,124,'Q');
 outTextXY(25,144,'P');
 setTextStyle(smallFont,horizDir,2);
 outTextXY(24,103,'F2');
 outTextXY(24,123,'Ctrl');
 outTextXY(23,163,'Esc');
 setColor(2);
 setTextStyle(smallFont,horizDir,4);
 outTextXY(41,101,'Sound');
 outTextXY(65,121,'Quit');
 outTextXY(41,141,'Pause');
 outTextXY(41,161,'End Game');

 setTextStyle(defaultFont,horizDir,1);
 outTextXY(254,24,'NEXT:');
 outTextXY(251,109,'SCORE:');
 outTextXY(256,120,'0');
 outTextXY(251,135,'LINES:');
 outTextXY(256,146,'0');
 outTextXY(251,160,'LEVEL:');
 outTextXY(256,171,'1');
end;  { draw1PlayerScreen }

procedure draw2PlayerScreen;
begin { draw2PlayerScreen }
 clearDevice;
 setFillStyle(ltBkSlashFill,1);
 bar(1,1,320,199);
 setFillStyle(emptyFill,0);
 bar(58,1,147,198);
 bar(173,1,262,198);

 setColor(3);
 setLineStyle(solidLn,0,thickWidth);
 rectangle(56,1,149,198);
 rectangle(54,1,151,198);
 rectangle(171,1,264,198);
 rectangle(169,1,266,198);

 setTextStyle(smallFont,horizDir,4);
 setColor(0); outTextXY(5,5,'PLAYER');
              outTextXY(272,5,'PLAYER');
 setColor(3); outTextXY(4,4,'PLAYER');
              outTextXY(271,4,'PLAYER');
 setTextStyle(triplexFont,horizDir,3);
 setColor(0); outTextXY(41,-2,'1');
              outTextXY(308,-2,'2');
 setColor(3); outTextXY(40,-3,'1');
              outTextXY(307,-3,'2');
 setTextStyle(defaultFont,horizDir,1);

 setColor(2);
 bar(4,30,49,85);
 bar(4,90,49,170);
 bar(271,30,316,85);
 bar(271,90,316,170);
 outTextXY(8,34,'NEXT:');
 outTextXY(8,94,'SCORE');
 outTextXY(8,105,'0');
 outTextXY(8,119,'LINES');
 outTextXY(8,130,'0');
 outTextXY(8,144,'LEVEL');
 outTextXY(8,155,'0');
 outTextXY(275,34,'NEXT:');
 outTextXY(275,94,'SCORE');
 outTextXY(275,105,'0');
 outTextXY(275,119,'LINES');
 outTextXY(275,130,'0');
 outTextXY(275,144,'LEVEL');
 outTextXY(275,155,'0');

end;  { draw2PlayerScreen }

procedure drawOpeningScreen;

begin { drawOpeningScreen }
 clearDevice;
 setFillStyle(ltBkSlashFill,1);
 bar(1,1,320,199);
 setLineStyle(solidLn,0,thickWidth);
 setFillStyle(emptyFill,0);
 bar(10,15,90,50);
 bar(120,15,294,120);
 bar(10,135,294,190);

 setColor(2);
 setTextStyle(triplexFont,horizDir,3);
 outTextXY(14,10,'TETRIS');
 line(14,36,86,36);
 setTextStyle(defaultFont,horizDir,1);
 outTextXY(15,41,'Lumpy ''95');

 setTextStyle(triplexFont,horizDir,1);
 outTextXY(125,12,'Select an option:');
 button(130,40,145,54);
 button(130,60,145,74);
 button(130,80,145,94);
 button(130,100,145,114);
 setTextStyle(defaultFont,horizDir,1);
 setColor(0);
 outTextXY(135,44,'1');
 outTextXY(135,64,'2');
 outTextXY(135,84,'K');
 setTextStyle(smallFont,horizDir,2);
 outTextXY(133,103,'Esc');
 setColor(2);
 setTextStyle(smallFont,horizDir,4);
 outTextXY(151,41,'Start 1 player game');
 outTextXY(151,61,'Start 2 player game');
 outTextXY(151,81,'Set the keys');
 outTextXY(151,101,'Exit');

 {bar(10,135,294,190);}
 setTextStyle(defaultFont,horizDir,1);
 outTextXY(15,138,'Keys during game:');
 button(35,152,50,166);
 button(35,172,50,186);
 button(170,152,185,166);
 button(170,172,190,186);
 button(195,172,210,186);

 setColor(0);
 outTextXY(40,176,'P');
 outTextXY(200,176,'Q');
 setTextStyle(smallFont,horizDir,2);
 outTextXY(40,155,'F2');
 outTextXY(173,155,'Esc');
 outTextXY(174,175,'Ctrl');
 setColor(2);
 setTextStyle(smallFont,horizDir,4);
 outTextXY(56,154,'Sound On/Off');
 outTextXY(56,174,'Pause');
 outTextXY(191,154,'End Game');
 outTextXY(216,174,'Exit');

end;  { drawOpeningScreen }

procedure drawKeySettingScreen;
var image : pointer;
    putType : byte;
    counter : byte;
begin { drawKeySettingScreen }
 clearDevice;
 setFillStyle(ltBkSlashFill,1);
 bar(1,1,320,199);
 setLineStyle(solidLn,0,thickWidth);
 setFillStyle(emptyFill,0);
 bar(10,15,90,50);
 bar(110,15,300,160);

 setColor(2);
 setTextStyle(triplexFont,horizDir,3);
 outTextXY(14,10,'TETRIS');
 line(14,36,86,36);
 setTextStyle(defaultFont,horizDir,1);
 outTextXY(15,41,'Lumpy ''95');

 outTextXY(130,25,'One Player');
 outTextXY(217,30,'Two Player');
 setLineStyle(solidLn,0,normWidth);
 line(205,35,205,45);
 line(267,40,267,45);
 rectangle(198,45,217,52);
 rectangle(243,45,292,52);
 setColor(0); line(197,52,295,52);
 setColor(2);
 setTextStyle(defaultFont,vertDir,1);
 outTextXY(243,60,'Player 1');
 outTextXY(273,60,'Player 2');
 setTextStyle(smallFont,horizDir,4);
 outTextXY(127,57,'Move left -');
 outTextXY(120,76,'Move right -');
 outTextXY(144,95,'Rotate -');
 outTextXY(154,114,'Drop -');
 button(200,56,215,70);
 button(200,75,215,89);
 button(200,94,215,108);
 button(200,113,215,127);
 button(245,56,260,70);
 button(245,75,260,89);
 button(245,94,260,108);
 button(245,113,260,127);
 button(275,56,290,70);
 button(275,75,290,89);
 button(275,94,290,108);
 button(275,113,290,127);

end;  { drawKeySettingScreen }

procedure makeOpeningScreenFile;
type extraBitType = array[1..4040] of byte;
var startFile : file of extraBitType;
    extraBit : extraBitType;
begin { makeOpeningScreenFile }
 drawOpeningScreen;
 assign(startFile,'start.dat');
 rewrite(startFile);
 getImage(10,135,294,190,extraBit);
 write(startFile,extraBit);
 setFillStyle(emptyFill,0);
 bar(10,135,294,190);
  outTextXY(13,135,'I don''t think there''s any need for me to explain');
  outTextXY(13,145,'what this game is all about...so I won''t!        ');
  outTextXY(13,157,'For those of you expecting VGA graphics and       ');
  outTextXY(13,167,'soundcard support, you''ll have to wait ''cause at');
  outTextXY(13,177,'the moment I''m stuck with an IBM XT!             ');
 getImage(10,135,294,190,extraBit);
 write(startFile,extraBit);
 bar(10,135,294,190);
  outTextXY(13,135,'There are some shortcut command line parameters');
  outTextXY(13,145,'      that you might find usefull:');
  outTextXY(13,158,'  /1        :  jump straight to one player game');
  outTextXY(13,168,'  /2        :  jump straight to two player game');
  outTextXY(13,178,'  /noSound  :  have a guess at this one!!');
 getImage(10,135,294,190,extraBit);
 write(startFile,extraBit);
 bar(10,135,294,190);
  outTextXY(13,135,'Written by Lumpy 26 September 1995');
  outTextXY(13,145,'You can get hold of me by...');
  outTextXY(13,160,'e-mail: p_egan@jim.ucc.ie or pe@student.ucc.ie');
  outTextXY(13,170,'WWW: http://interzone.ucc.ie/second/pfe/index.html');
  setTextStyle(smallFont,horizDir,3);
  outTextXY(13,182,'There''s no guarantee that these will be valid by the time you read this.');
 getImage(10,135,294,190,extraBit);
 write(startFile,extraBit);
 close(startFile);
end;  { makeOpeningScreenFile }

procedure showScreen(S : byte);
const screenSize = 16240;
type screenShotType = array[1..screenSize] of byte;
var onScreen : byte absolute $B800:0000;
    theScreen : screenShotType;
    screenFile : file of screenShotType;
begin { showScreen }
 assign(screenFile,'screens.dat');
 reset(screenFile);
  seek(screenFile,S);
  read(screenFile,theScreen);
 close(screenFile);

 move(theScreen,onScreen,screenSize);
end;  { showScreen }

procedure testOpeningScreen;
type writing = array[1..4040] of byte;
var startFile : file of writing;
    theWriting : writing;
begin { testOpeningScreen }
 showScreen(0);

 assign(startFile,'start.dat');
 reset(startFile);
  read(startFile,theWriting);
  putImage(10,135,theWriting,normalPut);
  readLn;
  read(startFile,theWriting);
  putImage(10,135,theWriting,normalPut);
 close(startFile);

end;  { testOpeningScreen }

procedure generateScreenFile;
const screenSize = 16240;
type screenShotType    = array[1..screenSize] of byte;
var onScreen : byte absolute $B800:0000;
    theScreen : screenShotType;
    screenFile : file of screenShotType;

begin { generateScreenFile }
 assign(screenFile,'screens.dat');
 rewrite(screenFile);
  drawOpeningScreen;
    move(onScreen,theScreen,screenSize);
    write(screenFile,theScreen);
  draw1PlayerScreen;
    move(onScreen,theScreen,screenSize);
    write(screenFile,theScreen);
  draw2PlayerScreen;
    move(onScreen,theScreen,screenSize);
    write(screenFile,theScreen);
  drawScoreScreen;
    move(onScreen,theScreen,screenSize);
    write(screenFile,theScreen);
  drawKeySettingScreen;
    move(onScreen,theScreen,screenSize);
    write(screenFile,theScreen);
 close(screenFile);
end;  { generateScreenFile }

begin { main }
 graphDriver := CGA; graphMode := CGAC1;
 initGraph(graphDriver,graphMode,'\tools\pascal\bgi');

 generateScreenFile;
 makeOpeningScreenFile;

 readLn;
 closeGraph;
end.  { main }
