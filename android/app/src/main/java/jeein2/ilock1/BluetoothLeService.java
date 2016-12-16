package jeein2.ilock1;

/**
 * Created by HANSUNG on 2016-11-07.
 * Master : 연결을 시도하고, 연결 후에 전체 connection을 관리하는 역할.
 Slave : Master의 연결 요청을 받고, Master의 timing 규약을 따르는 역할.
 Advertiser : Advertising Packet을 보내는 역할.
 * Scanner : Advertising Packet을 Scanning하는 역할. Scanner는 아래와 같은 2가지 Scanning 모드가 있다.
 * Passive Scanning : Scanner는 Advertising Packet을 받고 이에 대해 따로 응답을 보내지 않는다. 따라서 해당 Packet을 보낸 Advertiser는 Scanner가 Packet을 수신했는지에 대해서 알지 못한다.

 Active Scanning : Advertising Packet을 받은 Scanner는 Advertiser에게 추가적인 데이터를 요구하기 위해 *Scan Request라는 것을 보낸다. 이를 받은 Advertiser는 *Scan Response로 응답한다.

 * Scan Request, Scan Response : Advertising Packet type의 한 종류이다. 앞서, 31bytes 이하의 User data에 대해서는 Advertising Signal Packet에 넣어서 보낼 수 있다고 하였다. 하지만 31bytes보다는 크지만, Commection까지 맺어서 보내기는 오버헤드가 큰 데이터가 있을 때, Scan Request, Scan Response를 이용하면 두 번에 걸쳐서 데이터를 나눠 보낼 수 있게 된다. Advertising Packet을 받은 Scanner는 추가적인 User Data(예를 들어, Peripheral 디바이스의 이름)를 얻기 위해 Scan Request를 보내게 된다. Scan Request를 받은 Advertiser는 나머지 데이터를 Scan Response Signal에 담아서 보낸다.
 *
 *
 *
 * State
 Link Layer는 5가지 State를 가지고 있는데, 각 디바이스는 서로 연결이 되는 과정에서 이 State를 변화시킨다. 다음과 같은 5개의 State가 존재한다.

 Standby State : Signal Packet을 보내지도, 받지도 않는 상태.
 Advertising State : Advertising Packet을 보내고, 해당 Advertising Packet에 대한 상대 디바이스의 Response를 받을 수 있고 이에 응답할 수 있는 상태.
 Scanning State : Advertising Channel에서 Scaning하고 있는 상태.
 Initiating State : Advertiser의 Connectable Advertising Packet을 받고난 후 Connetion Request를 보내는 상태.
 Connection State : Connection 이후의 상태.
 *
 *3.3 Generic Access Profile (GAP)

 Generic Access Profile (GAP)는 서로 다른 제조사가 만든 BLE 디바이스들끼리 서로 호환되어 통신할 수 있도록 해주는 주춧돌 역할을 한다. 즉, 어떻게 디바이스간에 서로를 인지하고, Data를 Advertising하고, Connection을 맺을지에 대한 프레임워크를 제공한다. 그래서 GAP는 최상위 Control Layer라고도 불린다. Advertising Mode일 때, GAP에서 Advertising Data Payload와 Scan Response Payload를 포함할 수 있다.

 또한 GAP에서는 BLE 통신을 위해 Role, Mode, Procedure, Security, Additional GAP Data Format 등을 정의한다. 이들은 실제 API와 직접적으로 많은 연관이 있기 때문에 그 내용이 상당히 많지만, 여기서는 BLE Connection과 관련이 있는 Role에 대해서만 알아보겠다.

 Role
 Broadcaster : Link Layer에서 Advertiser 역할에 상응한다. 주기적으로 Advertising Packet을 보낸다. 예를 들면, 온도센서는 온도데이터를 자신과 연결된 디바이스에게 일정주기로 보낸다.
 Observer : Link Layer에서 Scanner 역할에 상응한다. Broadcaster가 뿌리는 Advertising Packet에서 data를 얻는다. 온도센서로부터 온도데이터를 받아서 디스플레이에 나타내는 테블릿 컴퓨터의 역할이다.
 Central : Link Layer에서 Master 역할과 상응한다. Central 역할은 다른 디바이스의 Advertising Packet을 듣고 Connection을 시작할 때 시작된다. 좋은 성능의 CPU를 가지고 있는 스마트폰이나, 테블릿 컴퓨터들의 역할이다.
 Peripheral : Link Layer에서 Master 역할과 상응한다. Advertising Packet을 보내서 Central 역할의 디바이스가 Connection을 시작할 수 있도록 하게끔 유도한다. 센서기능이 달린 디바이스들의 역할이다.



 3.4 Generic Attribute Profile (GATT)

 BLE Data 교환을 관리하는 GATT는 디바이스들이 Data를 발견하고, 읽고, 쓰는 것을 가능하게 하는 기초적인 Data Model과 Procedure를 정의한다. 그래서 GATT는 최상위 Data Layer라고도 불린다. 디바이스간에 low-level에서의 모든 인터렉션을 정의하는 GAP와는 달리, GATT는 오직 Data의 Format 및 전달에 대해서만 처리한다. Connection Mode일 때, GATT Service와 Characteristic을 이용하여 양방향 통신을 하게 된다. Service와 Characteristic에 대한 내용은 여기를 참고하길 바란다.

 GATT도 Data 처리와 관련해서 다음과 같은 역할을 정의한다.

 Role
 Client : Server에 Data를 요청한다. 하지만 처음에는 Server에 대해서 아는 것이 없기 때문에, Service Discovery라는 것을 수행한다. 이 후, Server에서 전송된 Response, Indication, Notification을 수신할 수 있다.
 Server : Client에게 Request를 받으면 Response를 보낸다. 또한 Client가 사용할 수 있는 User Data를 생성하고 저장해놓는 역할을 한다.






 Characteristic - 하나의 특성(characteristic)은 하나의 값과 n개의 디스크립터를 포함하는데 디스크립터는 특성 값을 기술한다. 하나의 특성은 클래스와 유사한 타입으로 생각하면 된다.
 Descriptor - 디스크립터는 특성의 값을 기술한다.
 Service - 하나의 서비스는 특성드의 집합니다. 예를 들어 "Heart Rate Monitor"라고 불리는 서비스를 가지고 있다면 그 서비스는 "heart rate measurement"같은 특성을 포함한다. GATT-based profile의 리스트와 서비스를 확인하려면 bluetooth.org를 방문하라.
 */
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattDescriptor;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.content.Context;
import android.content.Intent;
import android.os.Binder;
import android.os.IBinder;
import android.util.Log;

import java.io.UnsupportedEncodingException;
import java.util.List;

public class BluetoothLeService extends Service {
    private final static String TAG = BluetoothLeService.class.getSimpleName();

    private BluetoothManager mBluetoothManager; //매니저를 생성하면 ㅁ니저의 기능을 이용하여 어댑터에 블루투스장치를 시스템적으로 꺼내서 넣어주는 듯합니다.
    private BluetoothAdapter mBluetoothAdapter;//블루투스를 장치를 제어하기 위해 관리하는 객체
    BluetoothGatt mBluetoothGatt;//BLE Data 교환을 관리하는 GATT는 디바이스들이 Data를 발견하고, 읽고, 쓰는 것을 가능하게 하는 기초적인 Data Model
    public String mBluetoothDeviceAddress;

    private static final int STATE_DISCONNECTED = 0;
    private static final int STATE_CONNECTING = 1;
    private static final int STATE_CONNECTED = 2;
    public int mConnectionState = STATE_DISCONNECTED;


    //To tell the onCharacteristicWrite call back function that this is a new characteristic, 이 값에 따라 새로운 캐리터리스틱값인지 아닌지 알려줍니다.
    //not the Write Characteristic to the device successfully.
    private static final int WRITE_NEW_CHARACTERISTIC = -1;
    //define the limited length of the characteristic.
    private static final int MAX_CHARACTERISTIC_LENGTH = 17;
    //Show that Characteristic is writing or not.
    private boolean mIsWritingCharacteristic=false;

    //class to store the Characteristic and content string push into the ring buffer.
    private class BluetoothGattCharacteristicHelper{ //헬퍼클래스는 이곳에서자체 제작한 클래스다. 캐리터리스틱과 그 값들을 관리해주는 듯 하다.
        BluetoothGattCharacteristic mCharacteristic;
        String mCharacteristicValue;
        BluetoothGattCharacteristicHelper(BluetoothGattCharacteristic characteristic, String characteristicValue){
            mCharacteristic=characteristic;
            mCharacteristicValue=characteristicValue;
        }
    }
    //ring buffer
    private RingBuffer<BluetoothGattCharacteristicHelper> mCharacteristicRingBuffer = new RingBuffer<BluetoothGattCharacteristicHelper>(8);
    //앞서 정의한 링버퍼 클래스를 이용하여 아까 만들었던 헬퍼의 리스트로 만든다.
    public final static String ACTION_GATT_CONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_CONNECTED";
    public final static String ACTION_GATT_DISCONNECTED =
            "com.example.bluetooth.le.ACTION_GATT_DISCONNECTED";
    public final static String ACTION_GATT_SERVICES_DISCOVERED =
            "com.example.bluetooth.le.ACTION_GATT_SERVICES_DISCOVERED";
    public final static String ACTION_DATA_AVAILABLE =
            "com.example.bluetooth.le.ACTION_DATA_AVAILABLE";
    public final static String EXTRA_DATA =
            "com.example.bluetooth.le.EXTRA_DATA";
//    public final static UUID UUID_HEART_RATE_MEASUREMENT =
//            UUID.fromString(SampleGattAttributes.HEART_RATE_MEASUREMENT);

    // Implements callback methods for GATT events that the app cares about.  For example,
    // connection change and services discovered.
    /*
    첫번째 연동을 위한 과정은 BLE디바이스에 연결하는것인데 좀더 자세히 말하면 디바이스의 GATT서버에 연결하는 것이다. connectGatt()메소드로 하면 된다. 이 메소드는 3개의 파라메터가 있는데 context, autoConnect, 그리고 BluetoothGattCallback이 있다. autoConnect는 검색되었을 때 자동으로 연결할지에 대한 파라메터다.

mBluetoothGatt = device.connectGatt(this, false, mGattCallback);

이 코드는 BLE device를 GATT 서버 호스트로 연결한다. 그리고 BluetoothGatt 인스턴스를 반환한다. 이 인스턴스로 GATT client를 운영한다.
BluetoothGattCallback은 클라이언트에 연결상태나 client 운영에 대한 결과를 전달한다.

이 예제에서 BLE 앱은 하나의 액티비티가 연결하고 데이터를 표시하고 GATT 서비스와 특성들을 표시한다. 사용자 입력에 기반하여 BluetoothLeService로 불리는 서비스와 통신을 수행하도 Android BLE API를 통해 장치와 상호연동을 한다.
     */
    private final BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {//
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, int newState) {
            String intentAction;
            System.out.println("BluetoothGattCallback----onConnectionStateChange"+newState);
            if (newState == BluetoothProfile.STATE_CONNECTED) {
                intentAction = ACTION_GATT_CONNECTED;
                mConnectionState = STATE_CONNECTED;
                broadcastUpdate(intentAction); //브로드캐스트 업데이트 메서드느니오곳에서 정의한 인텐트 브로드캐스트 메서드입니다.
                Log.i(TAG, "Connected to GATT server.");
                // Attempts to discover services after successful connection.
                if(mBluetoothGatt.discoverServices())
                {
                    Log.i(TAG, "Attempting to start service discovery:");

                }
                else{
                    Log.i(TAG, "Attempting to start service discovery:not success");

                }


            } else if (newState == BluetoothProfile.STATE_DISCONNECTED) {
                intentAction = ACTION_GATT_DISCONNECTED;
                mConnectionState = STATE_DISCONNECTED;
                Log.i(TAG, "Disconnected from GATT server.");
                broadcastUpdate(intentAction);
            }
        }

        @Override
        public void onServicesDiscovered(BluetoothGatt gatt, int status) {//추측상 연결한뒤에 받을 수 있는 서비스를 검색하는 듯 하다.
            System.out.println("onServicesDiscovered "+status);
            if (status == BluetoothGatt.GATT_SUCCESS) {
                broadcastUpdate(ACTION_GATT_SERVICES_DISCOVERED);
            } else {
                Log.w(TAG, "onServicesDiscovered received: " + status);
            }
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt, BluetoothGattCharacteristic characteristic, int status) //데이터를 주고 읽는 핵심 메서드일듯
        {
            //this block should be synchronized to prevent the function overloading
            /*
            스레드는 같은 프로세스내에서 동일한 데이터를 공유한다. 따라서 하나의 데이터에 대해서 동시에 여러개의 스레드가 접근이 가능하게 되고 그에 따라 데이터의 일관성에 관한 문제가 생길 수가 있다.
그래서 자주 사용되는 것이 synchronized(동기화)이다. 동기화란 하나의 자원(데이터)에 대해서 여러 스레드가 사용하려고 할때 한 시점에서 하나의 스레드만 사용할 수 있도록 하는 것이다
             */
            synchronized(this)
            {
                //CharacteristicWrite success
                if(status == BluetoothGatt.GATT_SUCCESS)
                {
                    System.out.println("onCharacteristicWrite success:"+ new String(characteristic.getValue()));
                    if(mCharacteristicRingBuffer.isEmpty())
                    {
                        mIsWritingCharacteristic = false;
                    }
                    else
                    {
                        BluetoothGattCharacteristicHelper bluetoothGattCharacteristicHelper = mCharacteristicRingBuffer.next();
                        if(bluetoothGattCharacteristicHelper.mCharacteristicValue.length() > MAX_CHARACTERISTIC_LENGTH)
                        {
                            try {
                                bluetoothGattCharacteristicHelper.mCharacteristic.setValue(bluetoothGattCharacteristicHelper.mCharacteristicValue.substring(0, MAX_CHARACTERISTIC_LENGTH).getBytes("ISO-8859-1"));

                            } catch (UnsupportedEncodingException e) {
                                // this should never happen because "US-ASCII" is hard-coded.
                                throw new IllegalStateException(e);
                            }


                            if(mBluetoothGatt.writeCharacteristic(bluetoothGattCharacteristicHelper.mCharacteristic)) //writeCharacteristic는 하단에 정의되어 잇다.
                            {
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":success");
                            }else{
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":failure");
                            }
                            bluetoothGattCharacteristicHelper.mCharacteristicValue = bluetoothGattCharacteristicHelper.mCharacteristicValue.substring(MAX_CHARACTERISTIC_LENGTH);
                        }
                        else
                        {
                            try {
                                bluetoothGattCharacteristicHelper.mCharacteristic.setValue(bluetoothGattCharacteristicHelper.mCharacteristicValue.getBytes("ISO-8859-1"));
                            } catch (UnsupportedEncodingException e) {
                                // this should never happen because "US-ASCII" is hard-coded.
                                throw new IllegalStateException(e);
                            }

                            if(mBluetoothGatt.writeCharacteristic(bluetoothGattCharacteristicHelper.mCharacteristic))
                            {
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":success");
                            }else{
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":failure");
                            }
                            bluetoothGattCharacteristicHelper.mCharacteristicValue = "";

//	            			System.out.print("before pop:");
//	            			System.out.println(mCharacteristicRingBuffer.size());
                            mCharacteristicRingBuffer.pop();
//	            			System.out.print("after pop:");
//	            			System.out.println(mCharacteristicRingBuffer.size());
                        }
                    }
                }
                //WRITE a NEW CHARACTERISTIC
                else if(status == WRITE_NEW_CHARACTERISTIC)
                {
                    if((!mCharacteristicRingBuffer.isEmpty()) && mIsWritingCharacteristic==false)
                    {
                        BluetoothGattCharacteristicHelper bluetoothGattCharacteristicHelper = mCharacteristicRingBuffer.next();
                        if(bluetoothGattCharacteristicHelper.mCharacteristicValue.length() > MAX_CHARACTERISTIC_LENGTH)
                        {

                            try {
                                bluetoothGattCharacteristicHelper.mCharacteristic.setValue(bluetoothGattCharacteristicHelper.mCharacteristicValue.substring(0, MAX_CHARACTERISTIC_LENGTH).getBytes("ISO-8859-1"));
                            } catch (UnsupportedEncodingException e) {
                                // this should never happen because "US-ASCII" is hard-coded.
                                throw new IllegalStateException(e);
                            }

                            if(mBluetoothGatt.writeCharacteristic(bluetoothGattCharacteristicHelper.mCharacteristic))
                            {
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":success");
                            }else{
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":failure");
                            }
                            bluetoothGattCharacteristicHelper.mCharacteristicValue = bluetoothGattCharacteristicHelper.mCharacteristicValue.substring(MAX_CHARACTERISTIC_LENGTH);
                        }
                        else
                        {
                            try {
                                bluetoothGattCharacteristicHelper.mCharacteristic.setValue(bluetoothGattCharacteristicHelper.mCharacteristicValue.getBytes("ISO-8859-1"));
                            } catch (UnsupportedEncodingException e) {
                                // this should never happen because "US-ASCII" is hard-coded.
                                throw new IllegalStateException(e);
                            }


                            if(mBluetoothGatt.writeCharacteristic(bluetoothGattCharacteristicHelper.mCharacteristic))
                            {
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":success");
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[0]);
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[1]);
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[2]);
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[3]);
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[4]);
//	            	        	System.out.println((byte)bluetoothGattCharacteristicHelper.mCharacteristic.getValue()[5]);

                            }else{
                                System.out.println("writeCharacteristic init "+new String(bluetoothGattCharacteristicHelper.mCharacteristic.getValue())+ ":failure");
                            }
                            bluetoothGattCharacteristicHelper.mCharacteristicValue = "";

//		            			System.out.print("before pop:");
//		            			System.out.println(mCharacteristicRingBuffer.size());
                            mCharacteristicRingBuffer.pop();
//		            			System.out.print("after pop:");
//		            			System.out.println(mCharacteristicRingBuffer.size());
                        }
                    }

                    mIsWritingCharacteristic = true;

                    //clear the buffer to prevent the lock of the mIsWritingCharacteristic
                    if(mCharacteristicRingBuffer.isFull())
                    {
                        mCharacteristicRingBuffer.clear();
                        mIsWritingCharacteristic = false;
                    }
                }
                else
                //CharacteristicWrite fail
                {
                    mCharacteristicRingBuffer.clear();
                    System.out.println("onCharacteristicWrite fail:"+ new String(characteristic.getValue()));
                    System.out.println(status);
                }
            }
        }

        @Override
        public void onCharacteristicRead(BluetoothGatt gatt,
                                         BluetoothGattCharacteristic characteristic,
                                         int status) {
            if (status == BluetoothGatt.GATT_SUCCESS) {
                System.out.println("onCharacteristicRead  "+characteristic.getUuid().toString());
                broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
            }
        }
        @Override
        public void  onDescriptorWrite(BluetoothGatt gatt,
                                       BluetoothGattDescriptor characteristic,
                                       int status){
            System.out.println("onDescriptorWrite  "+characteristic.getUuid().toString()+" "+status);
        }
        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                                            BluetoothGattCharacteristic characteristic) {
            System.out.println("onCharacteristicChanged  "+new String(characteristic.getValue()));
            broadcastUpdate(ACTION_DATA_AVAILABLE, characteristic);
        }
    };

    private void broadcastUpdate(final String action) {
        final Intent intent = new Intent(action);
        sendBroadcast(intent);
    }

    private void broadcastUpdate(final String action,
                                 final BluetoothGattCharacteristic characteristic) {
        final Intent intent = new Intent(action);
        System.out.println("BluetoothLeService broadcastUpdate");
        // This is special handling for the Heart Rate Measurement profile.  Data parsing is
        // carried out as per profile specifications:
        // http://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.heart_rate_measurement.xml
//        if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
//            int flag = characteristic.getProperties();
//            int format = -1;
//            if ((flag & 0x01) != 0) {
//                format = BluetoothGattCharacteristic.FORMAT_UINT16;
//                Log.d(TAG, "Heart rate format UINT16.");
//            } else {
//                format = BluetoothGattCharacteristic.FORMAT_UINT8;
//                Log.d(TAG, "Heart rate format UINT8.");
//            }
//            final int heartRate = characteristic.getIntValue(format, 1);
//            Log.d(TAG, String.format("Received heart rate: %d", heartRate));
//            intent.putExtra(EXTRA_DATA, String.valueOf(heartRate));
//        } else {
        // For all other profiles, writes the data formatted in HEX.
        final byte[] data = characteristic.getValue();
        if (data != null && data.length > 0) {
            intent.putExtra(EXTRA_DATA, new String(data));
            sendBroadcast(intent);
        }
//        }
    }

    public class LocalBinder extends Binder {
        BluetoothLeService getService() {
            return BluetoothLeService.this;
        }
    }

    @Override
    public IBinder onBind(Intent intent) {
        return mBinder;
    }

    @Override
    public boolean onUnbind(Intent intent) {
        // After using a given device, you should make sure that BluetoothGatt.close() is called
        // such that resources are cleaned up properly.  In this particular example, close() is
        // invoked when the UI is disconnected from the Service.
        close();
        return super.onUnbind(intent);
    }

    private final IBinder mBinder = new LocalBinder();

    /**
     * Initializes a reference to the local Bluetooth adapter.
     *
     * @return Return true if the initialization is successful.
     */
    public boolean initialize() {
        // For API level 18 and above, get a reference to BluetoothAdapter through
        // BluetoothManager.
        System.out.println("BluetoothLeService initialize"+mBluetoothManager);
        if (mBluetoothManager == null) {
            mBluetoothManager = (BluetoothManager) getSystemService(Context.BLUETOOTH_SERVICE);
            if (mBluetoothManager == null) {
                Log.e(TAG, "Unable to initialize BluetoothManager.");
                return false;
            }
        }

        mBluetoothAdapter = mBluetoothManager.getAdapter();
        if (mBluetoothAdapter == null) {
            Log.e(TAG, "Unable to obtain a BluetoothAdapter.");
            return false;
        }

        return true;
    }

    /**
     * Connects to the GATT server hosted on the Bluetooth LE device.
     *
     * @param address The device address of the destination device.
     *
     * @return Return true if the connection is initiated successfully. The connection result
     *         is reported asynchronously through the
     *         {@code BluetoothGattCallback#onConnectionStateChange(android.bluetooth.BluetoothGatt, int, int)}
     *         callback.
     */
    public boolean connect(final String address) {//어드버타이징을 하는 BLE기기에 연결하려는 과정입니다.
        System.out.println("BluetoothLeService connect"+address+mBluetoothGatt);
        if (mBluetoothAdapter == null || address == null) {
            Log.w(TAG, "BluetoothAdapter not initialized or unspecified address.");
            return false;
        }

        // Previously connected device.  Try to reconnect. //이메서드는 이미 관계를 맺어쓰면 재연결 시도인데 지웠네.
//        if (mBluetoothDeviceAddress != null && address.equals(mBluetoothDeviceAddress)
//                && mBluetoothGatt != null) {
//            Log.d(TAG, "Trying to use an existing mBluetoothGatt for connection.");
//            if (mBluetoothGatt.connect()) {
//            	System.out.println("mBluetoothGatt connect");
//                mConnectionState = STATE_CONNECTING;
//                return true;
//            } else {
//            	System.out.println("mBluetoothGatt else connect");
//                return false;
//            }
//        }

        final BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
        if (device == null) {
            Log.w(TAG, "Device not found.  Unable to connect.");
            return false;
        }
        // We want to directly connect to the device, so we are setting the autoConnect
        // parameter to false.
        System.out.println("device.connectGatt connect");
        synchronized(this)
        {
            mBluetoothGatt = device.connectGatt(this, false, mGattCallback);
        }
        Log.d(TAG, "Trying to create a new connection.");
        mBluetoothDeviceAddress = address;
        mConnectionState = STATE_CONNECTING;
        return true;
    }

    /**
     * Disconnects an existing connection or cancel a pending connection. The disconnection result
     * is reported asynchronously through the
     * {@code BluetoothGattCallback#onConnectionStateChange(android.bluetooth.BluetoothGatt, int, int)}
     * callback.
     */
    public void disconnect() {
        System.out.println("BluetoothLeService disconnect");
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }
        mBluetoothGatt.disconnect();
    }

    /**
     * After using a given BLE device, the app must call this method to ensure resources are
     * released properly.
     */
    public void close() {
        System.out.println("BluetoothLeService close");
        if (mBluetoothGatt == null) {
            return;
        }
        mBluetoothGatt.close();
        mBluetoothGatt = null;
    }

    /**
     * Request a read on a given {@code BluetoothGattCharacteristic}. The read result is reported
     * asynchronously through the {@code BluetoothGattCallback#onCharacteristicRead(android.bluetooth.BluetoothGatt, android.bluetooth.BluetoothGattCharacteristic, int)}
     * callback.
     *
     * @param characteristic The characteristic to read from.
     */
    public void readCharacteristic(BluetoothGattCharacteristic characteristic) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }
        mBluetoothGatt.readCharacteristic(characteristic);
    }


    /**
     * Write information to the device on a given {@code BluetoothGattCharacteristic}. The content string and characteristic is
     * only pushed into a ring buffer. All the transmission is based on the {@code onCharacteristicWrite} call back function,
     * which is called directly in this function
     *
     * @param characteristic The characteristic to write to.
     */
    public void writeCharacteristic(BluetoothGattCharacteristic characteristic) {//쓰는 부분이네요.
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }

        //The character size of TI CC2540 is limited to 17 bytes, otherwise characteristic can not be sent properly,
        //so String should be cut to comply this restriction. And something should be done here:
        String writeCharacteristicString;
        try {
            writeCharacteristicString = new String(characteristic.getValue(),"ISO-8859-1");
        } catch (UnsupportedEncodingException e) {
            // this should never happen because "US-ASCII" is hard-coded.
            throw new IllegalStateException(e);
        }
        System.out.println("allwriteCharacteristicString:"+writeCharacteristicString);

        //As the communication is asynchronous content string and characteristic should be pushed into an ring buffer for further transmission
        mCharacteristicRingBuffer.push(new BluetoothGattCharacteristicHelper(characteristic,writeCharacteristicString) );
        System.out.println("mCharacteristicRingBufferlength:"+mCharacteristicRingBuffer.size());


        //The progress of onCharacteristicWrite and writeCharacteristic is almost the same. So callback function is called directly here
        //for details see the onCharacteristicWrite function
        mGattCallback.onCharacteristicWrite(mBluetoothGatt, characteristic, WRITE_NEW_CHARACTERISTIC);

    }

    /**
     * Enables or disables notification on a give characteristic.
     *
     * @param characteristic Characteristic to act on.
     * @param enabled If true, enable notification.  False otherwise.
     */
    public void setCharacteristicNotification(BluetoothGattCharacteristic characteristic,
                                              boolean enabled) {
        if (mBluetoothAdapter == null || mBluetoothGatt == null) {
            Log.w(TAG, "BluetoothAdapter not initialized");
            return;
        }
        mBluetoothGatt.setCharacteristicNotification(characteristic, enabled);

        //BluetoothGattDescriptor descriptor = characteristic.getDescriptor(characteristic.getUuid());
        //descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
        //mBluetoothGatt.writeDescriptor(descriptor);

        // This is specific to Heart Rate Measurement.
//        if (UUID_HEART_RATE_MEASUREMENT.equals(characteristic.getUuid())) {
//            BluetoothGattDescriptor descriptor = characteristic.getDescriptor(
//                    UUID.fromString(SampleGattAttributes.CLIENT_CHARACTERISTIC_CONFIG));
//            descriptor.setValue(BluetoothGattDescriptor.ENABLE_NOTIFICATION_VALUE);
//            mBluetoothGatt.writeDescriptor(descriptor);
//        }
    }

    /**
     * Retrieves a list of supported GATT services on the connected device. This should be
     * invoked only after {@code BluetoothGatt#discoverServices()} completes successfully.
     *
     * @return A {@code List} of supported services.
     */
    public List<BluetoothGattService> getSupportedGattServices() {
        if (mBluetoothGatt == null) return null;

        return mBluetoothGatt.getServices();
    }


}

