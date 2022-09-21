
                  { **** File Generator for Tetris **** }

program fileGenerator;
uses crt;

type byteFile = file of byte;


procedure makeBrickShapeFile;
var brickShapeFile : byteFile;

   procedure writeOut(x,y :byte);
   begin
     write(brickShapeFile,x,y);
   end; { writeOut }

begin { makeBrickShapeFIle }

 assign(brickShapeFile,'bricks.shp');
 rewrite(brickShapeFile);

  writeOut(0,0); writeOut(0,9);
  writeOut(9,9); writeOut(18,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(9,0); writeOut(9,9);
  writeOut(9,18); writeOut(18,0);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,9); writeOut(9,9);
  writeOut(18,9); writeOut(18,18);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,18); writeOut(9,0);
  writeOut(9,9); writeOut(9,18);
  writeOut(0,0); writeOut(18,27);

  writeOut(9,0); writeOut(0,9);
  writeOut(9,9); writeOut(18,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(9,0); writeOut(9,9);
  writeOut(9,18); writeOut(18,9);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,9); writeOut(9,9);
  writeOut(18,9); writeOut(9,18);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,9); writeOut(9,0);
  writeOut(9,9); writeOut(9,18);
  writeOut(0,0); writeOut(18,27);

  writeOut(18,0); writeOut(0,9);
  writeOut(9,9); writeOut(18,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(9,0); writeOut(9,9);
  writeOut(9,18); writeOut(18,18);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,9); writeOut(9,9);
  writeOut(18,9); writeOut(0,18);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,0); writeOut(9,0);
  writeOut(9,9); writeOut(9,18);
  writeOut(0,0); writeOut(18,27);

  writeOut(0,0); writeOut(0,9);
  writeOut(0,18); writeOut(0,27);
  writeOut(0,0); writeOut(9,35);
  writeOut(0,0); writeOut(9,0);
  writeOut(18,0); writeOut(27,0);
  writeOut(0,0); writeOut(35,9);
  writeOut(0,0); writeOut(0,9);
  writeOut(0,18); writeOut(0,27);
  writeOut(0,0); writeOut(9,35);
  writeOut(0,0); writeOut(9,0);
  writeOut(18,0); writeOut(27,0);
  writeOut(0,0); writeOut(35,9);

  writeOut(0,0); writeOut(0,9);
  writeOut(9,0); writeOut(9,9);
  writeOut(0,0); writeOut(18,18);
  writeOut(0,0); writeOut(0,9);
  writeOut(9,0); writeOut(9,9);
  writeOut(0,0); writeOut(18,18);
  writeOut(0,0); writeOut(0,9);
  writeOut(9,0); writeOut(9,9);
  writeOut(0,0); writeOut(18,18);
  writeOut(0,0); writeOut(0,9);
  writeOut(9,0); writeOut(9,9);
  writeOut(0,0); writeOut(18,18);

  writeOut(9,0); writeOut(18,0);
  writeOut(0,9); writeOut(9,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(0,0); writeOut(0,9);
  writeOut(9,9); writeOut(9,18);
  writeOut(0,0); writeOut(18,27);
  writeOut(9,0); writeOut(18,0);
  writeOut(0,9); writeOut(9,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(0,0); writeOut(0,9);
  writeOut(9,9); writeOut(9,18);
  writeOut(0,0); writeOut(18,27);

  writeOut(0,0); writeOut(9,0);
  writeOut(9,9); writeOut(18,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(9,9); writeOut(9,18);
  writeOut(18,0); writeOut(18,9);
  writeOut(0,0); writeOut(27,27);
  writeOut(0,0); writeOut(9,0);
  writeOut(9,9); writeOut(18,9);
  writeOut(0,0); writeOut(27,18);
  writeOut(9,9); writeOut(9,18);
  writeOut(18,0); writeOut(18,9);
  writeOut(0,0); writeOut(27,27);

 close(brickShapeFile);
end; { makeBrickShapeFile }

procedure makeBrickDataFile2;
var brickDataFile : byteFile;

    procedure writeOut(x,y : byte);
    begin
      write(brickDataFile,x,y);
    end; { writeOut }

begin { makeBrickDataFile2 };

 assign(brickDataFile,'bricks2.dat');
 rewrite(brickDataFile);

  writeOut(0,18); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(9,27); writeOut(17,9); writeOut(17,9); writeOut(26,9);
  writeOut(0,18); writeOut(9,18); writeOut(18,27); writeOut(26,27);
  writeOut(0,27); writeOut(9,27); writeOut(9,27); writeOut(17,27);

  writeOut(0,18); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(9,27); writeOut(17,27); writeOut(18,18); writeOut(26,18);
  writeOut(0,18); writeOut(9,27); writeOut(18,18); writeOut(26,18);
  writeOut(0,18); writeOut(9,27); writeOut(9,27); writeOut(17,27);

  writeOut(0,18); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(9,27); writeOut(18,27); writeOut(18,27); writeOut(26,27);
  writeOut(0,27); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(0,9); writeOut(9,27); writeOut(9,27); writeOut(17,27);

  writeOut(0,36); writeOut(2,36); writeOut(6,36); writeOut(8,36);
  writeOut(0,9); writeOut(9,9); writeOut(18,9); writeOut(34,9);
  writeOut(0,36); writeOut(2,36); writeOut(6,36); writeOut(8,36);
  writeOut(0,9); writeOut(9,9); writeOut(18,9); writeOut(34,9);

  writeOut(0,18); writeOut(9,18); writeOut(9,18); writeOut(17,18);
  writeOut(0,18); writeOut(9,18); writeOut(9,18); writeOut(17,18);
  writeOut(0,18); writeOut(9,18); writeOut(9,18); writeOut(17,18);
  writeOut(0,18); writeOut(9,18); writeOut(9,18); writeOut(17,18);

  writeOut(0,18); writeOut(9,18); writeOut(18,9); writeOut(26,9);
  writeOut(0,18); writeOut(9,27); writeOut(9,27); writeOut(17,27);
  writeOut(0,18); writeOut(9,18); writeOut(18,9); writeOut(26,9);
  writeOut(0,18); writeOut(9,27); writeOut(9,27); writeOut(17,27);

  writeOut(0,9); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(9,27); writeOut(18,18); writeOut(18,18); writeOut(26,18);
  writeOut(0,9); writeOut(9,18); writeOut(18,18); writeOut(26,18);
  writeOut(9,27); writeOut(18,18); writeOut(18,18); writeOut(26,18);

 close(brickDataFile);
end; { makeBrickDataFile2 }

procedure makeBrickDataFile;
var brickDataFile : byteFile;

    procedure writeOut(a,b,c,d : byte);
    begin
      write(brickDataFile,a,b,c,d);
    end; { writeOut }

begin { makeBrickDataFile };

 assign(brickDataFile,'bricks2.dat');
 rewrite(brickDataFile);
  writeOut(1,4,5,6); writeOut(2,3,5,8); writeOut(4,5,6,9); writeOut(2,5,7,8);
  writeOut(2,4,5,6); writeOut(2,5,6,8); writeOut(4,5,6,8); writeOut(2,4,5,8);
  writeOut(3,4,5,6); writeOut(2,5,8,9); writeOut(4,5,6,7); writeOut(1,2,5,8);
  writeOut(1,4,7,10); writeOut(1,2,3,11); writeOut(1,4,7,10); writeOut(1,2,3,11);
  writeOut(1,2,4,5); writeOut(1,2,4,5); writeOut(1,2,4,5); writeOut(1,2,4,5);
  writeOut(2,3,4,5); writeOut(1,4,5,8); writeOut(2,3,4,5); writeOut(1,4,5,8);
  writeOut(1,2,5,6); writeOut(3,5,6,8); writeOut(1,2,5,6); writeOut(3,5,6,8);

  writeOut(0,0,9,0); writeOut(18,0,0,9); writeOut(9,9,18,9);
  writeOut(0,18,9,18); writeOut(18,18,0,27); writeOut(27,0,0,0);
 close(brickDataFile);
end; { makeBrickDataFile }

procedure makeBlockDataFile;
var blockDataFile : byteFile;

    procedure writeOut(a,b,c,d : byte);
    begin
      write(blockDataFile,a,b,c,d);
    end; { writeOut }

begin { makeBlockDataFile };
 assign(blockDataFile,'bricks.dat');
 rewrite(blockDataFile);
  writeOut(0,0,9,0); writeOut(18,0,0,9); writeOut(9,9,18,9);
  writeOut(0,18,9,18); writeOut(18,18,0,27); writeOut(27,0,0,0);
 close(blockDataFile);
end;  { makeBlockDataFile }

procedure makeBrickDataFile3;
type blockPoint = record X,Y : byte; end;
const blockPointArray : array[1..11] of blockPoint =
      ((X:0; Y:0), (X:9; Y:0), (X:18;Y:0),
       (X:0; Y:9), (X:9; Y:9), (X:18;Y:9),
       (X:0; Y:18),(X:9; Y:18),(X:18;Y:18),
       (X:0; Y:27),(X:27;Y:0));

var brickDataFile1, brickDataFile2 : file of byte;
    x : byte;

begin
 assign(brickDataFile1,'bricks1.dat');
  assign(brickDataFile2,'bricks2.dat');
  reset(brickDataFile2);
   rewrite(brickDataFile1);
    while not eof(brickDataFile2) do begin
     read(brickDataFile2,x);
     write(brickDataFile1,blockPointArray[x].X,blockPointArray[x].Y);
    end;
   close(brickDataFile1);
  close(brickDataFile2);
end;  { makeBrickDataFile3 }

procedure makeKeyFile;
type playerKeyType = array[0..2] of string[4];
const funcKey = #0;
      outString : array[1..4] of string[5] = ('ROTATE','DROP','LEFT','RIGHT');
var inKey : char;
    playerKeys : playerKeyType;
    keyFile : file of playerKeyType;
    x,y : byte;

begin { makeKeyFile }
 clrScr;

 assign(keyFile,'keys.dat');
 rewrite(keyFile);

 for y := 0 to 2 do begin
  playerKeys[y] := '';
  writeln('Enter player ',y,' keys:');
  for x := 1 to 4 do begin
   writeln(outString[x]);
   inkey := readkey;
   if inkey = funcKey then inkey := chr(ord(readkey) + 127);
   playerKeys[y] := concat(playerKeys[y],inkey);
  end;
 end;

 write(keyFile,playerKeys);
 close(keyFile);

end;  { makeKeyFile }


begin { theMainBit }
 {makeBrickShapeFile;}
 {makeBrickDataFile;}
 {makeBrickDataFile2;}
 {makeBlockDataFile;}
 makeKeyFile;
end.  { theMainBit }
