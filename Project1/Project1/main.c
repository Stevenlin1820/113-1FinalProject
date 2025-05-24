package tetris;

import java.io.*;
import java.net.Socket;
import java.util.Arrays;

public class TetrisTestTCPClient {

    private Socket socket;
    private InputStream in;
    private OutputStream out;

    public void connect(String host, int port) throws IOException {
        socket = new Socket(host, port);
        in = socket.getInputStream();
        out = socket.getOutputStream();
    }

    public void close() throws IOException {
        if (socket != null) socket.close();
    }

    public void sendCommand(String cmd) throws IOException {
        // 傳送文字指令，需加換行符號
        String commandWithNewline = cmd + "\n";
        out.write(commandWithNewline.getBytes("UTF-8"));
        out.flush();
    }

    /**
     * 從伺服器讀取回傳的遊戲狀態：
     * 1 byte: boolean isGameOver (0 or 1)
     * 4 bytes: int removedLines
     * 4 bytes: int pngSize
     * pngSize bytes: PNG圖片資料
     */
    public GameResponse readResponse() throws IOException {
        // 讀1 byte boolean
        int isOverByte = in.read();
        if (isOverByte == -1) throw new EOFException("End of stream reached while reading isOver");

        boolean isGameOver = (isOverByte != 0);

        // 讀4 bytes 整數(移除行數)
        int removedLines = readInt(in);

        // 讀4 bytes png大小
        int pngSize = readInt(in);
        if (pngSize < 0 || pngSize > 1024 * 1024) // 防止異常png size
            throw new IOException("Invalid PNG size: " + pngSize);

        // 讀pngSize bytes圖片資料
        byte[] pngData = new byte[pngSize];
        readFully(in, pngData);

        return new GameResponse(isGameOver, removedLines, pngData);
    }

    // 工具方法：讀4字節整數(大端)
    private int readInt(InputStream in) throws IOException {
        byte[] buf = new byte[4];
        readFully(in, buf);
        return ((buf[0] & 0xFF) << 24) |
            ((buf[1] & 0xFF) << 16) |
            ((buf[2] & 0xFF) << 8) |
            (buf[3] & 0xFF);
    }

    // 工具方法：確保讀取指定長度資料
    private void readFully(InputStream in, byte[] buf) throws IOException {
        int offset = 0;
        while (offset < buf.length) {
            int read = in.read(buf, offset, buf.length - offset);
            if (read == -1)
                throw new EOFException("Unexpected end of stream");
            offset += read;
        }
    }

    /**
     * 輔助類別，用來封裝伺服器回傳的遊戲狀態
     */
    public static class GameResponse {
        public final boolean isGameOver;
        public final int removedLines;
        public final byte[] pngData;

        public GameResponse(boolean isGameOver, int removedLines, byte[] pngData) {
            this.isGameOver = isGameOver;
            this.removedLines = removedLines;
            this.pngData = pngData;
        }
    }

    // 測試用主函式(選擇性)
    public static void main(String[] args) {
        TetrisTestTCPClient client = new TetrisTestTCPClient();
        try {
            client.connect("localhost", 12345);
            client.sendCommand("start");
            GameResponse resp = client.readResponse();

            System.out.println("Game over: " + resp.isGameOver);
            System.out.println("Removed lines: " + resp.removedLines);
            System.out.println("PNG image size: " + resp.pngData.length);

            // 可存檔查看圖片
            try (FileOutputStream fos = new FileOutputStream("received.png")) {
                fos.write(resp.pngData);
            }

            client.close();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
    }
}