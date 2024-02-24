package se.liu.albhe576.project;

public class Vector<T>
{
    private T [] data;
    public T getX(){
        return this.data[0];
    }
    public T getY(){
        return this.data[1];
    }
    public T getZ(){
        return this.data[2];
    }
    public T getW(){
        return this.data[3];
    }
    public Vector(int size){
        this.data = (T[]) new Object[size];
    }
    public Vector(T x, T y, T z){
        this.data = (T[]) new Object[]{x,y,z};

    }
    public Vector(T x, T y){
        this.data = (T[]) new Object[]{x,y};

    }

    @Override public String toString() {
        return this.data[0] + " " + this.data[1] + " " + this.data[2];
    }

    public static float crossProduct2D(Vector<Short> v0, Vector<Short> v1, Vector<Short> point){
        int v0v1x = v1.getX() - v0.getX();
        int v0v1y = v1.getY() - v0.getY();

        int v0px = point.getX() - v0.getX();
        int v0py = point.getY() - v0.getY();

        return v0v1x * v0py - v0v1y * v0px;

    }

    public static Vector<Float> normalize(Vector<Float>v){
        float x = v.getX();
        float y = v.getY();
        float z = v.getZ();
        float sum = (float) Math.sqrt(x * x + y * y + z * z);
        return new Vector<>(
                x / sum,
                y / sum,
                z / sum
        );
    }
    public static float dot(Vector<Float> v0, Vector<Float> v1) {
        return v0.getX() * v1.getX() + v0.getY() * v1.getY() + v0.getZ() * v1.getZ();
    }
    public static Vector<Float> cross(Vector<Float> v0, Vector<Float> v1) {
        return new Vector<>(
                v0.getY() * v1.getZ() - v0.getZ() * v1.getY(),
                v0.getZ() * v1.getX() - v0.getX() * v1.getZ(),
                v0.getX() * v1.getY() - v0.getY() * v1.getX()
        );
    }

    public static Vector<Float> subtract(Vector<Float> v0, Vector<Float> v1){
        return new Vector<>(
            v0.getX() - v1.getX(),
            v0.getY() - v1.getY(),
            v0.getZ() - v1.getZ()
        );
    }
    public Vector(T x, float y, float z, float w){
        this.data = (T[]) new Object[]{x,y,z,w};
    }

    public static void main(String[] args) {
    }
}
