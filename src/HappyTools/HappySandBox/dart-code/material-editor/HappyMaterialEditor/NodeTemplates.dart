#library("HappyNodeHelpers");

#source('NodeDescriptor.dart');

class NodeTemplates 
{
  static List<NodeDescriptor> templates = new List<NodeDescriptor>();
  static Map<String, NodeDescriptor> shortcuts = new Map<String, NodeDescriptor>();
  
  static NodeDescriptor fromShortcut(String shortcut)
  {
    if (shortcuts.containsKey(shortcut))
      return shortcuts[shortcut];
    else
      return null;
  }
  
  static void init()
  {
    initAbs();
    initAdd();
    initFloat();
    initFloat2();
    initFloat3();
    initFloat4();
    
    templates.forEach(addShortcut);
  }
  
  static void addShortcut(NodeDescriptor desc)
  {
    if (desc.shortcut.isEmpty() == false) 
      shortcuts[desc.shortcut.toLowerCase()] = desc;
  }
  
  // Math
  static void initAbs()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Abs", templates.length);
    
    // fill
    desc.AddFormatedNames(" : ");
    desc.AddFormatedOverload("f:f");
    desc.AddFormatedOverload("f2:f2");
    desc.AddFormatedOverload("f3:f3");
    desc.AddFormatedOverload("f4:f4");
       
    // fin
    templates.add(desc);
  }
  static void initAdd()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Add", templates.length);
    
    // fill
    desc.AddFormatedNames("A,B: ");
    desc.AddFormatedOverload("f,f:f");
    desc.AddFormatedOverload("f2,f2:f2");
    desc.AddFormatedOverload("f2,f:f2");
    desc.AddFormatedOverload("f3,f3:f3");
    desc.AddFormatedOverload("f3,f:f3");
    desc.AddFormatedOverload("f4,f4:f4");
    desc.AddFormatedOverload("f4,f:f4");
    
    //shortcut
    desc.shortcut = "A";
    
    // fin
    templates.add(desc);
  }
  static void initMultiply()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Multiply", templates.length);

    // fill
    desc.AddFormatedNames("A,B: ");
    desc.AddFormatedOverload("f,f:f");
    desc.AddFormatedOverload("f2,f2:f2");
    desc.AddFormatedOverload("f2,f:f2");
    desc.AddFormatedOverload("f3,f3:f3");
    desc.AddFormatedOverload("f3,f:f3");
    desc.AddFormatedOverload("f4,f4:f4");
    desc.AddFormatedOverload("f4,f:f4");
    
    //shortcut
    desc.shortcut = "M";
    
    // fin
    templates.add(desc);
  }
  
  // Constants
  static void initFloat()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Float", templates.length);
    
    // fill
    desc.AddFormatedNames(": ");
    desc.AddFormatedOverload(":f");
    
    // parameters
    ParameterDescriptor param;
    param = new ParameterDescriptor("value", ValueTypes.Float);
    
    //shortcut
    desc.shortcut = "1";
    
    // fin
    templates.add(desc);
  }
  static void initFloat2()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Float2", templates.length);

    // fill
    desc.AddFormatedNames(": ");
    desc.AddFormatedOverload(":f2");
    
    // parameters
    ParameterDescriptor param;
    param = new ParameterDescriptor("value", ValueTypes.Float2);

    //shortcut
    desc.shortcut = "2";
    
    // fin
    templates.add(desc);
  }
  static void initFloat3()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Float3", templates.length);

    // fill
    desc.AddFormatedNames(": ");
    desc.AddFormatedOverload(":f3");
    
    // parameters
    ParameterDescriptor param;
    param = new ParameterDescriptor("value", ValueTypes.Float3);
    
    //shortcut
    desc.shortcut = "3";
    
    // fin
    templates.add(desc);
  }
  static void initFloat4()
  {
    NodeDescriptor desc;
    ConnectionOverload overload;
    // desc
    desc = new NodeDescriptor("Float4", templates.length);

    //shortcut
    desc.shortcut = "4";

    // fill
    desc.AddFormatedNames(": ");
    desc.AddFormatedOverload(":f4");
    
    // parameters
    ParameterDescriptor param;
    param = new ParameterDescriptor("value", ValueTypes.Float4);
    
    // fin
    templates.add(desc);
  }
  
  
//  static const NodeDescriptor AppendVector="AppendVector"; 
//  static const NodeDescriptor BumpOffset="BumpOffset"; 
//  static const NodeDescriptor CameraVector="CameraVector"; 
//  static const NodeDescriptor CameraWorldPosition="CameraWorldPosition"; 
//  static const NodeDescriptor Ceil="Ceil"; 
//  static const NodeDescriptor Clamp="Clamp"; 
//  static const NodeDescriptor ComponentMask="ComponentMask"; 
//  static const NodeDescriptor Constant="Constant"; 
//  static const NodeDescriptor Constant2Vector="Constant2Vector"; 
//  static const NodeDescriptor Constant3Vector="Constant3Vector"; 
//  static const NodeDescriptor Constant4Vector="Constant4Vector"; 
//  static const NodeDescriptor ConstantBiasScale="ConstantBiasScale"; 
//  static const NodeDescriptor ConstantClamp="ConstantClamp"; 
//  static const NodeDescriptor Cosine="Cosine"; 
//  static const NodeDescriptor CrossProduct="CrossProduct"; 
//  static const NodeDescriptor Custom="Custom"; 
//  static const NodeDescriptor CustomTexture="CustomTexture"; 
//  static const NodeDescriptor DepthBiasBlend="DepthBiasBlend"; 
//  static const NodeDescriptor DepthBiasAlpha="DepthBiasAlpha"; 
//  static const NodeDescriptor DepthBiasedBlend="DepthBiasedBlend"; 
//  static const NodeDescriptor DeriveNormalZ="DeriveNormalZ"; 
//  static const NodeDescriptor Desaturation="Desaturation"; 
//  static const NodeDescriptor DestColor="DestColor"; 
//  static const NodeDescriptor DestDepth="DestDepth"; 
//  static const NodeDescriptor Distance="Distance"; 
//  static const NodeDescriptor Divide="Divide"; 
//  static const NodeDescriptor DotProduct="DotProduct"; 
//  static const NodeDescriptor FlipBookSample="FlipBookSample"; 
//  static const NodeDescriptor Floor="Floor"; 
//  static const NodeDescriptor FMod="FMod"; 
//  static const NodeDescriptor FontSampler="FontSampler"; 
//  static const NodeDescriptor Frac="Frac"; 
//  static const NodeDescriptor Fresnel="Fresnel"; 
//  static const NodeDescriptor If="If"; 
//  static const NodeDescriptor Lerp="Lerp"; 
//  static const NodeDescriptor Multiply="Multiply"; 
//  static const NodeDescriptor Normalize="Normalize"; 
//  static const NodeDescriptor ObjectWorldPosition="ObjectWorldPosition"; 
//  static const NodeDescriptor OneMinus="OneMinus"; 
//  static const NodeDescriptor Panner="Panner"; 
//  static const NodeDescriptor PerInstanceRandom="PerInstanceRandom"; 
//  static const NodeDescriptor PixelDepth="PixelDepth"; 
//  static const NodeDescriptor Power="Power"; 
//  static const NodeDescriptor ReflectionVector="ReflectionVector"; 
//  static const NodeDescriptor Rotator="Rotator"; 
//  static const NodeDescriptor RotateAboutAxis="RotateAboutAxis"; 
//  static const NodeDescriptor SceneDepth="SceneDepth"; 
//  static const NodeDescriptor SceneTexture="SceneTexture"; 
//  static const NodeDescriptor ScreenPosition="ScreenPosition"; 
//  static const NodeDescriptor Sine="Sine"; 
//  static const NodeDescriptor SphereMask="SphereMask"; 
//  static const NodeDescriptor SquareRoot="SquareRoot"; 
//  static const NodeDescriptor Subtract="Subtract"; 
//  static const NodeDescriptor TextureCoordinate="TextureCoordinate"; 
//  static const NodeDescriptor Texture2D="Texture2D"; 
//  static const NodeDescriptor TextureCube="TextureCube"; 
//  static const NodeDescriptor Time="Time"; 
//  static const NodeDescriptor WorldPosition="WorldPosition"; 
//  static const NodeDescriptor WorldNormal="WorldNormal"; 
//  static const NodeDescriptor VertexColor="VertexColor"; 
//  static const NodeDescriptor WindDirectionAndSpeed="WindDirectionAndSpeed"; 
}
