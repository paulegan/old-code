program bricksLoadTest;
uses crt, graph;


type blockPoint = record X,Y : byte; end;
     brickType = record
                  image : array[0..230] of byte;
                  blockPos : array[0..3] of blockPoint;
                 end;
     brickTypeArray = array[1..7,0..3] of brickType;

const blockPointArray : array[1..11] of blockPoint =
      ((X:0; Y:0), (X:9; Y:0), (X:18;Y:0),
       (X:0; Y:9), (X:9; Y:9), (X:18;Y:9),
       (X:0; Y:18),(X:9; Y:18),(X:18;Y:18),
       (X:0; Y:27),(X:27;Y:0));

var graphDriver : integer;
    graphMode : integer;
    x,y,z : byte;
    brick : brickTypeArray;

procedure initBricks;
const drawAtPos : pointType = (X:100; Y:100);
var brickShapeFile : file of byte;
    brickDataFile : file of byte;
    X1,Y1,X2,Y2 : byte;
    block : array[1..7] of pointer;

  procedure drawBlocks;
  begin { drawBlocks }
   for x := 1 to 7 do
    getMem(block[x],42);

   setLineStyle(solidLn,0,normWidth); setColor(2);
   setFillStyle(solidFill,1);
   rectangle(130,10,138,18);
   floodFill(133,13,2);
   rectangle(132,12,136,16);
   getImage(130,10,138,18,block[1]^);

   putImage(130,25,block[1]^,notPut);
   putImage(130,25,block[1]^,notPut);
   getImage(130,25,138,33,block[3]^);

   setFillStyle(solidFill,1);
   bar(130,40,138,48);
   setColor(0); rectangle(132,42,136,46);
   setColor(3); line(132,42,132,46);
                line(132,42,136,42);
   getImage(130,40,138,48,block[2]^);

   block[4] := block[1];

   setFillStyle(solidFill,2);
   bar(130,55,137,62);
   line(130,55,130,63);
   line(130,55,138,55);
   getImage(130,55,138,63,block[5]^);

   setFillStyle(solidFill,2);
   bar(130,70,138,78);
   setColor(1);
   line(130,71,130,78);
   line(138,70,138,77);
   line(130,78,138,70);
   line(130,71,138,77);
   setFillStyle(solidFill,1);
   floodFill(131,74,1);
   floodFill(137,74,1);
   setFillStyle(solidFill,3);
   bar(133,73,135,75);
   getImage(130,70,138,78,block[6]^);

   bar(130,85,138,93);
   getImage(130,85,138,93,block[7]^);
   putImage(130,85,block[6]^,normalPut);
   putImage(130,85,block[7]^,XORPut);
   bar(133,88,135,90);
   getImage(130,85,138,93,block[7]^);

   setFillStyle(emptyFill,0);
   bar(130,5,140,100);
  end;  { drawBlocks }

  procedure drawExtraThings;
  var image : pointer;
  begin { drawExtraThings }
   setColor(2);
   setFillStyle(interleaveFill,2);
   bar(131,1,137,34);
   bar(131,46,164,52);
   bar(131,91,137,124);
   bar(131,136,164,142);

   setFillStyle(interleaveFill,1);
   bar(31,161,37,195);
   getMem(image,imageSize(31,161,37,195));
   getImage(31,161,37,195,image^);
   putImage(131,0,image^,XORPut);
   line(131,0,137,0);
   putImage(131,90,image^,XORPut);
   line(131,90,137,90);
   freeMem(image,imageSize(31,161,37,195));

   bar(51,186,84,193);
   getMem(image,imageSize(51,186,84,193));
   getImage(51,186,84,193,image^);
   putImage(131,45,image^,XORPut);
   line(131,45,164,45);
   putImage(131,135,image^,XORPut);
   line(131,135,164,135);
   freeMem(image,imageSize(51,186,84,193));

  end;  { drawExtraThings }

begin { initBricks }

 drawBlocks;

 { draw and get all the bricks: }
 assign(brickShapeFile,'bricks.shp');
 reset(brickShapeFile);
 for x := 1 to 7 do
  for y := 0 to 3 do begin
   for z := 0 to 3 do begin
    read(brickShapeFile,X1,Y1);
    putImage(X1+(x*40)-30,Y1+(y*45),block[x]^,normalPut);
   end;
   read(brickShapeFile,X1,Y1,X2,Y2);
   getImage(X1+(x*40)-30,Y1+(y*45),X2+(x*40)-31,Y2+(y*45)-1, brick[x,y].image);
  end;
 close(brickShapeFile);

 drawExtraThings;

 reset(brickShapeFile);
 for x := 1 to 7 do
  for y := 0 to 3 do begin
   for z := 0 to 3 do read(brickShapeFile,X1,Y1);
   read(brickShapeFile,X1,Y1,X2,Y2);
   getImage(X1+(x*40)-30,Y1+(y*45),X2+(x*40)-30,Y2+(y*45), brick[x,y].image);
  end;
 close(brickShapeFile);

 assign(brickDataFile,'bricks1.dat');
 reset(brickDataFile);
 for x := 1 to 7 do
  for y := 0 to 3 do
   for z := 0 to 3 do begin
    read(brickDataFile,X1,Y1);
    brick[x,y].blockPos[z].X := X1;
    brick[x,y].blockPos[z].Y := Y1;
   end;
 close(brickDataFile);

end;  { initBricks }

procedure showBricks;
begin { showBricks }
 setFillStyle(solidFill,3);
 for x := 1 to 7 do
  for y := 0 to 3 do begin
   bar(240,40,290,90);
   putImage(250,50,brick[x,y].image,normalPut);
   for z := 0 to 3 do
    putPixel(250+brick[x,y].blockPos[z].X,50+brick[x,y].blockPos[z].Y,3);
   if readKey = #27 then exit;
  end;
end;  { showBricks }

procedure createBrickFile;
var brickFile : file of brickTypeArray;
begin { createBrickFile }
 assign(brickFile,'bricks.dat');
 rewrite(brickFile);
  write(brickFile,brick);
 close(brickFile);
end;  { createBrickFile }

procedure readBricksFromFile;
var brickFile : file of brickTypeArray;
begin { readBricksFormFile }
 assign(brickFile,'bricks.dat');
 reset(brickFile);
  read(brickFile,brick);
 close(brickFile);

end;  { readBricksFromFile }

begin { main }
 graphDriver := CGA; graphMode := CGAC1;
 initGraph(graphDriver,graphMode,'\tools\pascal\bgi');

 initBricks;
 createBrickFile;
 {
 readBricksFromFile;
 showBricks;
 }

 readln;
 closeGraph;
end.  { main }