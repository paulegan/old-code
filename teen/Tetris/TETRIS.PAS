
                     { ****** TETRIS - Lumpy '95 ****** }

                     {       written by Paul Egan       }
                     {      27/8/1995 -> 29/9/1995      }

program tetris;
{$M 22000,0,655360}

uses crt, graph, lumpysLibrary;

type blockPoint = record X,Y : byte; end;
     brickType = record
                  image : array[0..230] of byte;
                  blockPos : array[0..3] of blockPoint;
                 end;
     brickTypeArray = array[1..7,0..3] of brickType;
     playerKeyType = array[0..2] of string[4];
     aSound = (S_rotate,S_move,S_land,S_lineGot);

var brick : brickTypeArray;
    playerKeys : playerKeyType;
    x,y,z : byte;          { counters }
    soundOff : boolean;
    gamePicked : byte;

procedure initialize;
var brickFile : file of brickTypeArray;
    keyFile : file of playerKeyType;
    graphDriver : integer;
    graphMode : integer;

begin { initialize }
 writeLn('Loading TETRIS...');

 checkForFile('screens.dat');
 checkForFile('scores.dat');
 checkForFile('start.dat');

 checkForFile('bricks.dat');
 assign(brickFile,'bricks.dat');
 reset(brickFile);
  read(brickFile,brick);
 close(brickFile);

 checkForFile('keys.dat');
 assign(keyFile,'keys.dat');
 reset(keyFile);
  read(keyFile,playerKeys);
 close(keyFile);

 if registerBGIdriver(@CGADriverProc) < 0 then halt;
 graphDriver := CGA; graphMode := CGAC1;
 initGraph(graphDriver,graphMode,'');
 clearDevice;
 randomize;
 gamePicked := 0;
end;  { initialize }

procedure quitNow;
begin { quitNow }
  closeGraph;
  writeLn('Thanks for playing.');
  halt;
end;  { quitNow }

procedure parseParameters;
var testString : string;

  function tidiedUp(S : string) : string;
  begin { tidiedUp }
   if (S[1] = '-') or (S[1] = '/') then delete(S,1,1);
   for y := 1 to length(S) do S[y] := upCase(S[y]);
   tidiedUp := S;
  end;  { tidiedUp }

begin { parseParameters }
 for x := 1 to paramCount do begin
  testString := tidiedUp(paramStr(x));
  soundOff := testString = 'NOSOUND';
  if (testString='1') or (testString='2') then
   gamePicked := ord(testString[1]) - 48;
 end;
end;  { parseParameters }

procedure showScreen(S : byte);
type screenShotType = array[1..screenSize] of byte;
var theScreen : screenShotType;
    screenFile : file of screenShotType;
begin { showScreen }
 assign(screenFile,'screens.dat');
 reset(screenFile);
  seek(screenFile,S);
  read(screenFile,theScreen);
 close(screenFile);

 clearDevice;
 move(theScreen,onScreen,screenSize);
end;  { showScreen }

procedure goOpeningScreen;
type changingBitType = array[1..4040] of byte;
var startFile : file of changingBitType;
    changingBit : changingBitType;
    inKey : char;
    counter : word;
    cBit : byte;

  procedure changeTheKeys;
  const BPos : array[0..2] of integer = (200,245,275);
        otherKeys : set of char = ['P','p',#187,#27,#17];
  var lastPos : pointType;
      inKey : char;
      outChar : char;
      keyFile : file of playerKeyType;
      keyUsed : boolean;

    procedure button(X,Y : integer;buttonUp : boolean);
    var C : shortInt;
    begin { button }
     if buttonUp then C := 0 else C := 3;
     setLineStyle(solidLn,0,normWidth);
     setColor(C); rectangle(X,Y,X+15,Y+14);
                  rectangle(X+1,Y+1,X+14,Y+13);
     setColor(C-3); line(X,Y,X+15,Y); line(X+1,Y+1,X+14,Y+1);
                    line(X,Y,X,Y+14); line(X+1,Y+1,X+1,Y+13);
    end;  { button }

  begin { changeTheKeys }
   showScreen(4);
   repeat
    setFillStyle(solidFill,2);
    for x := 0 to 2 do
     for y := 1 to 4 do
      bar(BPos[x]+2,39+(y*19),BPos[x]+13,49+(y*19));
    setFillStyle(solidFill,3);
    lastPos.X := BPos[1]; lastPos.Y := 56;
    for x := 0 to 2 do
     playerKeys[x] := '';
    for x := 0 to 2 do
     for y := 1 to 4 do begin
      repeat
       lastPos.X := BPos[x]; lastPos.Y := 37+(y*19);
       button(lastPos.X,lastPos.Y,false);
       inKey := readKey;
       if inKey = #0 then inKey := chr(ord(readKey) + 127);
       case inKey of
        #199 : outChar := chr(24);
        #207 : outChar := chr(25);
        #202 : outChar := chr(27);
        #204 : outChar := chr(26);
        #32  : outChar := chr(95);
        #13  : outChar := chr(20);
        else   outChar := upCase(inKey);
       end;

       keyUsed := false;
       if inKey in otherKeys then keyUsed := true;
       if pos(inKey,playerKeys[x]) <> 0 then keyUsed := true;
       if (x = 2) and (pos(inKey,playerKeys[1]) <> 0) then keyUsed := true;
       if keyUsed then
        messageBox('This key has already been used.')
       else begin
        button(lastPos.X,lastPos.Y,true);
        setColor(0);
        outTextXY(lastPos.X+4,lastPos.Y+4,outChar);
        playerKeys[x] := concat(playerKeys[x],inkey);
       end;

      until not(keyUsed);
     end; {for}
    messageBox('Are these OK?');
   until upCase(mBoxResponse) <> 'N';
   assign(keyFile,'keys.dat');
   rewrite(keyFile);
   write(keyFile,playerKeys);
   close(keyFile);

   showScreen(0);
  end;  { changeTheKeys }

begin { goOpeningScreen }
 showScreen(0);

 assign(startFile,'start.dat');
 reset(startFile);

 cBit := 3;
 repeat
  counter := 1;
  cBit := succ(cBit) mod 4;
  seek(startFile,cBit);
  read(startFile,changingBit);
  putImage(10,135,changingBit,normalPut);
  repeat
   inc(counter);
   delay(3);
   if keyPressed then begin
    inKey := readKey;
    if inKey = #13 then inKey := '1';
    case inKey of
     '1','2' : gamePicked := ord(inKey) - 48;
     'K','k' : changeTheKeys;
     #27,#17 : begin                { escape & control-Q }
                close(startFile);
                quitNow;
               end;
    end; {case}
   end; {if}
  until (counter = 1500) or (gamePicked <> 0)
 until gamePicked <> 0;
 close(startFile);
end;  { goOpeningScreen }

procedure playGame(gameType : byte);

type playerType = record
                   currentBrick : byte;
                   nextBrick : byte;
                   brickDir : byte;
                   nextBrickDir : byte;
                   XPos,YPos : byte;
                   YPosInc : byte;
                   theScore : longInt;
                   linesGot : word;
                   level : byte;
                  end;
var player : array[0..2] of playerType;
    delayTime : byte;
    gameFinished : boolean;
    firstPlayer : byte;
    lastPlayer : byte;

  procedure playSound(theSound : aSound; doStop : boolean);
  var counter : integer;
  begin { playSound }
   if soundOff then exit;
   case theSound of
    S_rotate : begin
                 for counter := 1 to 100 do begin
                  sound(counter*8);
                  delay(1);
                 end;
               end;
    S_move   : begin
                 for counter := 1 to 20 do begin
                  sound(601-counter*6);
                  delay(2);
                 end;
               end;
    S_land   : begin
                 for counter := 1 to 100 do begin
                  sound(601-counter*6);
                  delay(1);
                 end;
               end;
    S_lineGot: begin
                 sound(10000);
                 sound(Random(1000-y));
               end;
   end; { case }
   if doStop then NoSound;
  end;  { playSound }


  procedure dealWithKey;
  const funcKey = #0;
        notPlayer = 5;
  var inKey : char;
      origXPos : byte;
      origBrickDir : byte;
      playerNum : byte;
      keyPos : byte;

  begin { dealWithKey }
    inKey := readKey;
    if inKey = funcKey then inKey := chr(ord(readKey) + 127);

    playerNum := notPlayer;
    for x := firstPlayer to lastPlayer do
     if pos(inKey,playerKeys[x]) > 0 then playerNum := x;

    if playerNum <> notPlayer then
     with player[playerNum] do begin
      origXPos := XPos;  origBrickDir := brickDir;
      keyPos := pos(inKey,playerKeys[playerNum]);
      case keyPos of
       1 : begin
            dec(XPos,9);
            playSound(S_move,true);
           end;
       2 : begin
            inc(XPos,9);
            playSound(S_move,true);
           end;
       3 : begin
            brickDir := (brickDir + 1) mod 4;
            playSound(S_rotate,true);
           end;
       4 : begin
            for x := 1 to 2 do if (YPos-8) mod 9 <> 0 then inc(YPos,3);
            YPosInc := 9;
           end;
      end;

      { check to see if brick will overlap anything: }
      for x := 0 to 3 do
       if getPixel(XPos+brick[currentBrick,brickDir].blockPos[x].X+4,
                   YPos+brick[currentBrick,brickDir].blockPos[x].Y+2)
                    <> 0 then begin
                     XPos := origXPos;
                     brickDir := origBrickDir;
                    end;
     end; {with}

    if playerNum = notPlayer then
     case inKey of
      'p','P': messageBox('Press any key to continue...');
      #187: soundOff := not(soundOff);    { F2 }
      #27: gameFinished := true;          { escape }
      #17: quitNow;                       { control-Q }
     end;

  end;  { dealWithKey }

  procedure getNextBrickForPlayer(playerNum : byte);
  const outPos : array[0..2] of pointType = ((X:255;Y:35),(X:9;Y:45),(X:276;Y:45));
  begin { getNextBrickForPlayer }
   with player[playerNum] do begin
    currentBrick := nextBrick;
    brickDir := nextBrickDir;
    nextBrick := random(7) + 1;
    nextBrickDir := random(3);
    with outPos[playerNum] do begin
     clearBox(X,Y,X+35,Y+35);
     putImage(X,Y,brick[nextBrick,nextBrickDir].image,XORPut);
    end;
   end;
  end;  { getNextBrickForPlayer }

  procedure checkIfaBrickHasLanded;
  var brickHasLanded : boolean;
      playerNum : byte;

    procedure checkForLine;
    const sides : array[0..2] of pointType = ((X:106;Y:213),(X:58;Y:147),(X:173;Y:262));
    var numLines : byte;
        line : byte;
        Ystart,Yend : byte;
        leftSide, rightSide : integer;

      procedure doScoreStuff;
      const outPos : array[0..2] of pointType = ((X:256;Y:120),(X:8;Y:105),(X:274;Y:105));
      begin { doScoreStuff }
       with player[playerNum] do begin
        inc(linesGot,numLines);
        inc(theScore,(200*numLines)-100);
        if numLines > 3 then inc(theScore,300);

        with outPos[playerNum] do begin
         clearBox(X-2,Y-2,X+40,Y+12);
         outTextXY(X,Y,num2str(theScore));
         clearBox(X-2,Y+23,X+40,Y+37);
         outTextXY(X,Y+25,num2str(linesGot));
        end;

        if linesGot mod 10 = 0 then begin         { new level }
         inc(level);
         with outPos[playerNum] do begin
          clearBox(X-2,Y+48,X+40,Y+62);
          outTextXY(X,Y+50,num2str(level));
         end;
         delayTime := 84 - (level*4);
        end;

       end;  { with }
      end;  { doScoreStuff }

      procedure explodeLine;
      var theImage : pointer;
      begin { explodeLine }
       getMem(theImage,imageSize(leftSide-10,Yend-(4*numLines),
                                rightSide+10,Yend+(12*numLines)));
       getImage(leftSide-10,Yend-(4*numLines),
               rightSide+10,Yend+(12*numLines),theImage^);
       for x := 1 to 4 do
        for y := 1 to (90-(5*x)) do begin
         playSound(S_lineGot,false);
         putPixel(leftSide+8-(4*x) + random(rightSide-leftSide-16+(8*x)),
                  Yend+(4*numLines)-(2*x*numLines)+random(4*x*numLines),
                  random(4));
        end;
       noSound;
       putImage(leftSide-10,Yend-(4*numLines),theImage^,normalPut);
       freeMem(theImage,imageSize(leftSide-10,Yend-(4*numLines),
                                 rightSide+10,Yend+(12*numLines)));
       clearBox(leftSide,Ystart-1,rightSide,Yend);

       getMem(theImage,imageSize(leftSide,10,rightSide,Yend-1));
         getImage(leftSide,10,rightSide,Yend-1,theImage^);
         putImage(leftSide,10+Ystart-Yend,theImage^,normalPut);
       freeMem(theImage,imageSize(leftSide,10,rightSide,Yend-1));
      end;  { explodeLine }

      function fullLine(line : byte) : boolean;
      var booleTemp : boolean;
      begin { fullLine }
       booleTemp := true;
       for x := 1 to (12-lastPlayer) do {I know, I know... its crude!}
        if getPixel(leftSide-7+(x*9),player[playerNum].YPos+39-(line*9)) = 0
         then booleTemp := false;
       fullLine := booleTemp;
      end;  { fullLine }

    begin { checkForLine }
      leftSide := sides[playerNum].X;
      rightSide := sides[playerNum].Y;
      Ystart := player[playerNum].YPos + 36;
      Yend := Ystart;

      for line := 1 to 5 do
       if fullLine(line) then dec(Yend,9)
       else begin
        if Yend <> Ystart then begin            { fill line found : }
         numLines := round((Ystart-Yend)/9);
         explodeLine;
         inc(player[playerNum].YPos,Ystart-Yend);
         doScoreStuff;
        end;
        Ystart := player[playerNum].YPos + 36 - (line*9);
        Yend := Ystart;
       end;
    end;  { checkForLine }


  begin { checkIfaBrickHasLanded }
    for playerNum := firstPlayer to lastPlayer do
     with player[playerNum] do begin

      brickHasLanded := false;
      for x := 0 to 3 do
       if getPixel(XPos+brick[currentBrick,brickDir].blockPos[x].X,
                   YPos+brick[currentBrick,brickDir].blockPos[x].Y+9)
                    <> 0 then brickHasLanded := true;

      if brickHasLanded then begin
       playSound(S_land,true);
       if (YPos < 20) then gameFinished := true;
       putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
       checkForLine;
       case playerNum of
        0 : XPos := 151;
        1 : XPos := 85;
        2 : XPos := 200;
       end;
       YPos := 8;
       YPosInc := 3;
       getNextBrickForPlayer(playerNum);
      end;

     end; {with}
  end;  { checkIfaBrickHasLanded }


  procedure initVariables;
  begin { initVariables }
   delayTime := 80;
   gameFinished := false;
   for x := firstPlayer to lastPlayer do
    with player[x] do begin
     theScore := 0; linesGot := 0; level := 1;
     case x of
      0 : XPos := 151;
      1 : XPos := 85;
      2 : XPos := 200;
     end;
     YPos := 8;
     YPosInc := 3;
     nextBrickDir := random(3);
     nextBrick := random(7) + 1;
     getNextBrickForPlayer(x);
    end;
  end;  { initVariables }

  procedure play1PlayerGame;
  begin { play1PlayerGame }
   initVariables;
   clearBox(106,3,213,196);

    repeat
     with player[0] do begin
      inc(YPos,YPosInc);
      putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
      delay(delayTime);
      putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
     end;

     if keyPressed then dealWithKey;
     checkIfaBrickHasLanded;
    until gameFinished;
    messageBox('  GAME  OVER  ');
  end;  { play1PlayerGame }

  procedure play2PlayerGame;
  begin { play2PlayerGame }
    initVariables;

    repeat
     inc(player[1].YPos,player[1].YPosInc);
     inc(player[2].YPos,player[2].YPosInc);
     with player[1] do putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
     with player[2] do putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
     delay(delayTime);
     with player[1] do putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);
     with player[2] do putImage(XPos,YPos,brick[currentBrick,brickDir].image,XORPut);

     if keyPressed then dealWithKey;
     checkIfaBrickHasLanded;
    until gameFinished;
    messageBox('  GAME  OVER  ');
  end;  { play2PlayerGame }


begin { playGame }
 showScreen(gameType);
 messageBox('Press any key to continue...');
 case gameType of
  1 : begin
       firstPlayer := 0; lastPlayer := 0;
       play1PlayerGame;
       delay(1000);
       showScreen(3);
       goScoreBoard(player[0].theScore);
      end;
  2 : begin
       firstPlayer := 1; lastPlayer := 2;
       play2PlayerGame;
      end;
 end;
 gamePicked := 0;
 delay(3000);
end;  { playGame }


begin { main }
 initialize;
 parseParameters;

 if gamePicked <> 0 then playGame(gamePicked);
 repeat
  goOpeningScreen;
  playGame(gamePicked);
 until false;

end.  { main }