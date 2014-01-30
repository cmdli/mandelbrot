//Christopher de la Iglesia

import javax.swing.JFrame;

import java.awt.event.MouseEvent;
import java.awt.event.MouseListener;
import java.awt.event.KeyEvent;
import java.awt.event.MouseListener;
import java.awt.image.BufferedImage;
import java.awt.Graphics;
import java.awt.Color;
import java.awt.MouseInfo;
import java.util.Hashtable;
import java.util.Random;
import java.awt.image.DataBufferInt;

public class Mandelbrot extends JFrame implements MouseListener {

	private static final double UNBOUNDED = 4.0;
	private double VIEW_X = 0.0;
	private double VIEW_Y = 0.0;
	private double VIEW_W = 2.0;
	private double VIEW_H = 2.0;
	private static final int MAX_ITERATION = 2000;

	private Hashtable<Integer,Integer> colors;
    private Graphics draw;
    private BufferedImage buffer;
    private int[] pixels;
    private int w = 600;
    private int h = 600;
    private int steps;
    private int mouseX = 0;
    private int mouseY = 0;
    private int mouseStartX = 0;
    private int mouseStartY = 0;
    private boolean mousePressed = false;

    public Mandelbrot() {
		super("Mandelbrot");
		super.setSize(w,h);
		super.setDefaultCloseOperation(EXIT_ON_CLOSE);
		super.setVisible(true);
		steps = 0;
		buffer = new BufferedImage(w,h,BufferedImage.TYPE_INT_ARGB);
		pixels = ((DataBufferInt) buffer.getRaster().getDataBuffer()).getData();
		draw = getGraphics();
		addMouseListener(this);
		genColors();
		genMandelbrot();
    }

    //Main thread
    public static void main(String[] args) {
    	Mandelbrot m = new Mandelbrot();
    	while(true) {
            m.update();
    		try {
    			Thread.sleep(30);
    		}
			catch(Exception e) {}
    	}
    }

    //Updates the Screen
    public void update() {
		draw.drawImage(buffer,0,0,null);
		draw.setColor(Color.blue);
		if(mousePressed)
    		draw.drawRect(mouseStartX, mouseStartY, MouseInfo.getPointerInfo().getLocation().x-mouseStartX, MouseInfo.getPointerInfo().getLocation().x-mouseStartX);
    }

    public void genMandelbrot() {
		for(int x = 0; x < w; x++) {
		    for(int y = 0; y < h; y++) {
		    	pixels[x+y*w] = 0xFF000000 | mandel(x,y);
		    }
		}
		for(int y = 0; y < h; y++) {
    		pixels[w/2 + y*w] = 0xFFFFFFFF;
    	}
    	for(int x = 0; x < w; x++) {
    		pixels[x + (h/2)*w] = 0xFFFFFFFF;
    	}
    }

    public void genColors() {
    	colors = new Hashtable<Integer,Integer>();
    	for(int i = 0; i < MAX_ITERATION; i++) {
    		colors.put(i, i%256 | ((256 - i%256) * 0x000100));
    	}
    	colors.put(MAX_ITERATION,0x000000);
    }

	public int mandel(int x0, int y0) {
		double x = ((x0-(double)w/2.0)/(double)w*2.0)*VIEW_W + VIEW_X;
		double y = ((y0-(double)h/2.0)/(double)h*2.0)*VIEW_H + VIEW_Y;
    	double a = 0.0;
    	double atmp = 0.0;
    	double b = 0.0;
    	int iteration = 0;
		while(a*a+b*b < UNBOUNDED && iteration < MAX_ITERATION) {
			atmp = 2.0*a*b + y;
			b = b*b-a*a + x;
			a = atmp;
			iteration++;
		}
		return colors.get(iteration);
    }

	public void mousePressed(MouseEvent arg0) {
		mousePressed = true;
		mouseStartX = arg0.getX();
		mouseStartY = arg0.getY();
	}

	public void mouseReleased(MouseEvent arg0) {
		mousePressed = false;
		double startX = ((mouseStartX-(double)w/2.0)/(double)w*2.0)*VIEW_W + VIEW_X;
		double startY = ((mouseStartY-(double)h/2.0)/(double)h*2.0)*VIEW_H + VIEW_Y;
		double endX = ((arg0.getX()-(double)w/2.0)/(double)w*2.0)*VIEW_W + VIEW_X;
		double endY = endX-startX + startY;
		VIEW_X = (endX-startX)/2.0 + startX;
		VIEW_Y = (endY-startY)/2.0 + startY;
		VIEW_W = (endX-startX)/2.0;
		VIEW_H = (endY-startY)/2.0;
		genMandelbrot();
	}

	@Override
	public void mouseClicked(MouseEvent arg0) {
		VIEW_X = 0.0;
		VIEW_Y = 0.0;
		VIEW_W = 2.0;
		VIEW_H = 2.0;
		genMandelbrot();
	}

	@Override
	public void mouseEntered(MouseEvent arg0) {}
	@Override
	public void mouseExited(MouseEvent arg0) {}
}

