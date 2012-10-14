#library("HappyNodes");

#import('dart:html');
#import('NodeTemplates.dart');
#import('HappyFramework.dart');

#source('Node.dart');

class NodeView 
{
  List m_NodeContainer = new List();
  String m_Key = "";
  
  var m_SelectedNode = null;
  var m_SelectedConnecter = null;
  
  static const int NO_DRAG = 0;
  static const int CAN_DRAG = 1;
  static const int IS_DRAGGING = 2;
  static const int DRAG_SENSITIVITY = 4;
  int m_DragState = NO_DRAG;
  vec2 m_StartDragPosition = new vec2(0.0, 0.0);
  vec2 m_DragOffset = new vec2(0.0, 0.0);
  vec2 m_MousePosition = new vec2(0.0, 0.0);
  
  NodeView()
  {
    NodeTemplates.init();
  }
  // Create
  void createNode(NodeDescriptor desc, int x, int y)
  {
    Node node = new Node(new vec2(x.toDouble(), y.toDouble()));
    desc.inputNames.forEach(f(e) => node.addInput(new Connecter(e)));
    desc.outputNames.forEach(f(e) => node.addOutput(new Connecter(e)));
    m_NodeContainer.add(node);
  }
  
  // Events
  void OnKeyPressed(KeyboardEvent e)
  {
    m_Key = new String.fromCharCodes([e.charCode]);
  }
  void OnKeyReleased(KeyboardEvent e)
  {
    m_Key = "";
  }
  
  void Deselect()
  {
    if (m_SelectedNode != null)
    {
      m_SelectedNode.m_IsSelected = false;
      m_SelectedNode = null;
      m_DragState = NO_DRAG;
    } 
  }
  void OnMousePressed(MouseEvent e)
  {
    if (m_Key.isEmpty())
    {
      double xClick = e.layerX.toDouble();
      double yClick = e.layerY.toDouble();
      num count = m_NodeContainer.length;
      Deselect();
      for (int i = 0; i < count; i++)
      {
        if (m_NodeContainer[i].collisionCheck(xClick, yClick))
        {
          m_SelectedNode = m_NodeContainer[i];
          m_SelectedNode.m_IsSelected = true;
          m_DragState = CAN_DRAG;
          m_StartDragPosition.x = xClick;
          m_StartDragPosition.y = yClick;
          m_DragOffset = m_SelectedNode.m_Position - m_StartDragPosition;
          break;
        }
      }
    }
    else
    {
      NodeDescriptor desc = NodeTemplates.fromShortcut(m_Key);
      if (desc != null)
      {
        createNode(desc, e.layerX, e.layerY);
      }
    }
  }
  void OnMouseReleased(MouseEvent e)
  {
    m_DragState = NO_DRAG;
  }
  void OnMouseMove(MouseEvent e)
  {
    m_MousePosition.x = e.layerX.toDouble();
    m_MousePosition.y = e.layerY.toDouble();
    
    if (m_DragState == CAN_DRAG)
    {
      if ((m_StartDragPosition - m_MousePosition).length() > DRAG_SENSITIVITY)
        m_DragState = IS_DRAGGING;
    }
    else if (m_DragState == IS_DRAGGING)
    {
      m_SelectedNode.m_Position = m_MousePosition + m_DragOffset;
    }
  }
  
  void draw(CanvasRenderingContext2D context)
  {
    m_NodeContainer.forEach((n) => n.draw(context));
    
    StringBuffer buff = new StringBuffer();
    buff.add("Mouse: ");
    buff.add(m_MousePosition.x.toStringAsFixed(2));
    buff.add(", ");
    buff.add(m_MousePosition.y.toStringAsFixed(2));
    
    context.fillText(buff.toString(), 12, 12);
  }
}
