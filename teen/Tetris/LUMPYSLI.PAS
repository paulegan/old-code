                     {   Lumpy's procedure library  }
                     {     written by Paul Egan     }
                     {    last updated 18/9/1995    }

unit lumpysLibrary;

interface

const screenSize = 16240;
var onScreen : byte absolute $B800:0000;
    mBoxResponse : char;

function num2str(theNumber : longInt) : string;
procedure CGADriverProc;
procedure clearBox(X1,Y1,X2,Y2 : integer);
procedure checkForFile(fileName : string);
procedure messageBox(message : string);
procedure goScoreBoard(theScore : longInt);

implementation
uses crt, graph;

function num2str(theNumber : longInt) : string;
var strTemp : string;
begin { num2str }
 str(theNumber,strTemp);
 num2str := strTemp;
end;  { num2str }

procedure CGADriverProc; external;
{$L CGA.OBJ }

procedure clearBox(X1,Y1,X2,Y2 : integer);
var origSettings : fillSettingsType;
begin { clearBox }
 getFillSettings(origSettings);
 setFillStyle(emptyFill,0);
 bar(X1,Y1,X2,Y2);
 setFillStyle(origSettings.pattern,origSettings.color);
end;  { clearBox }


procedure checkForFile(fileName : string);
var testFile : file;
begin { checkForFile }
 assign(testFile,fileName);
  {$I-}
   Reset(testFile);
  {$I+}
  if IOResult <> 0 then begin
   WriteLn('You seem to be missing a file: ',fileName);
   halt(2);
  end
  else
   Close(testFile);
end;  { checkForFile }


procedure messageBox(message : string);
var background : pointer;
    halfBoxLength : byte;
begin { messageBox }
 halfBoxLength := round((8 * length(message) + 10) / 2);
 getMem(background,imageSize(160-halfBoxLength,120,160+halfBoxLength,140));
 getImage(160-halfBoxLength,120,160+halfBoxLength,140,background^);

 bar(160-halfBoxLength,120,160+halfBoxLength,140);
 setLineStyle(solidLn,0,normWidth); setColor(0);
 rectangle(162-halfBoxLength,122,158+halfBoxLength,138);
 outTextXY(167-halfBoxLength,127,message);
 setColor(1);
 outTextXY(166-halfBoxLength,126,message);
 setColor(2);

 mBoxResponse := readKey;
 if mBoxResponse = #0 then mBoxResponse := chr(ord(readKey) + 127);
 if mBoxResponse = #17 then begin    { control-Q }
  closeGraph;
  halt;
 end;

 putImage(160-halfBoxLength,120,background^,normalPut);
 freeMem(background,imageSize(160-halfBoxLength,120,160+halfBoxLength,140));
end;  { messageBox }


procedure goScoreBoard(theScore : longInt);
type personScoreType = record
                        name : string[19];
                        score : longInt;
                       end;
var personScore : array[1..15] of personScoreType;
    scoreFile : file of personScoreType;
    placing : byte;
    player : byte;
    playersName : string[19];
    background : pointer;

  procedure getBoardFromFile;
  var player : byte;
  begin { getBoardFromFile }
   assign(scoreFile,'scores.dat');
   reset(scoreFile);
   for player := 1 to 15 do begin
    read(scoreFile,personScore[player]);
    outTextXY(110,10+player*10,personScore[player].name);
    outTextXY(267,10+player*10,num2str(personScore[player].score));
   end;
   close(scoreFile);
  end;  { getBoardFromFile }

begin { goScoreBoard }

 getBoardFromFile;

 placing := 0;          { sort: }
 for player := 15 downto 1 do
  if theScore > personScore[player].score then placing := player;

 if placing = 0 then
  messageBox('You need more practice!')
 else begin            { got on the score board }
  getMem(background,imageSize(55,60,265,140));
  getImage(55,60,265,140,background^);
   bar(55,60,265,140);
   setLineStyle(solidLn,0,normWidth); setColor(0);
   rectangle(57,62,263,138);
   outTextXY(119,70,'Well Done!');
   outTextXY(67,85,'You''re in position no '+num2str(placing));
   outTextXY(73,108,'Enter your name below:');
   setColor(2); outTextXY(118,69,'Well Done!');
   setColor(1);
   outTextXY(66,84,'You''re in position no '+num2str(placing));
   outTextXY(72,107,'Enter your name below:');
   clearBox(70,118,250,128);
   gotoXY(10,16); readln(playersName);
  putImage(55,60,background^,normalPut);
  freeMem(background,imageSize(55,60,265,140));

  for player := 15 downto placing+1 do
   personScore[player] := personScore[player-1];
  personScore[placing].name := playersName;
  personScore[placing].score := theScore;
  clearBox(100,8+placing*10,310,170);
  for player := placing to 15 do begin
   if player = placing+1 then setColor(2);
   outTextXY(110,10+player*10,personScore[player].name);
   outTextXY(267,10+player*10,num2str(personScore[player].score));
  end;

  rewrite(scoreFile);
  for player := 1 to 15 do
   write(scoreFile,personScore[player]);
  close(scoreFile);

 end;  { else if }
end;  { goScoreBoard }

end. { implementation }