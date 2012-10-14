#library("HappyMaterialEditor");

#import('dart:html');
#import('dart:math');
#import('NodeView.dart');

num rotatePos = 0;

CanvasElement canvas;
CanvasRenderingContext2D context;
NodeView nodeView;

void main() 
{  
  canvas = query("#editorCanvas");
  context = canvas.context2d;
  
  nodeView = new NodeView();
  
  canvas.on.mouseDown.add(nodeView.OnMousePressed);
  canvas.on.mouseUp.add(nodeView.OnMouseReleased);
  canvas.on.mouseMove.add(nodeView.OnMouseMove);
  window.on.keyPress.add(nodeView.OnKeyPressed, true);
  window.on.keyUp.add(nodeView.OnKeyReleased, true);
  
  window.requestAnimationFrame(tick);

}

bool tick(int time)
{
  context.clearRect(0,0,canvas.width,canvas.height);
  nodeView.draw(context);
  window.requestAnimationFrame(tick);
}

