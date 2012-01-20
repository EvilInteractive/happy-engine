namespace HappyFxEditorBaseLib.Behaviour
{
    public enum ParticleSystemBehaviour
    {
        Color = 0,      
        Force = 1,          
        Location = 2,       
        Rotation = 3,       
        RotationRate = 4,   
        Scale = 5,          
        Speed = 6,          

        Required = 7,

        MAX
    }
    public enum CameraFxBehaviour
    {
        Shake = 0,
        Flash,

        Required,

        MAX
    }
    public enum AudioFxBehaviour
    {
        Volume = 0,
        Pan,
        Pitch,

        Required,

        MAX
    }
}
