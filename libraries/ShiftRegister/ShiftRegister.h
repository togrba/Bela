#pragma once
#include <Bela.h> // this class acts on the BelaContext directly
#include <vector>

/**
 * @brief A class to drive a shift register using Bela's digital outputs.
 *
 * A class to drive a shift register using Bela's digital outputs.
 * This class accesses the BelaContext directly to perform data output.
 * You need to assign it three digital channels at initialisation, to which
 * data will be written at each call to process().
 */
class ShiftRegister
{
public:
	/**
	 * The Bela digital pins connected to the shift register.
	 */
	struct Pins {
		unsigned int data; ///< data pin
		unsigned int clock; ///< clock pin
		unsigned int latch; ///< latch pin
	};
	/**
	 * Default constructor. Does nothing. If the object is constructed
	 * this way, you have to call setup() to initialise it properly.
	 */
	ShiftRegister();
	/**
	 * Set the Bela digital channels that will be used and the maximum
	 * length of the messages to be transmitted.
	 *
	 * @param pins the data, clock, latch pins to be used.
	 * @param maxSize the maximum length of the messages that will be
	 * passed to setData().
	 */
	ShiftRegister(const Pins& pins, unsigned int maxSize);
	/**
	 * \copydoc ShiftRegister::ShiftRegister(unsigned int, unsigned int, unsigned int, unsigned int)
	 */
	 void setup(const Pins& pins, unsigned int maxSize);
	/**
	 * Check whether the last data set with setData() has been fully shifted out.
	 *
	 * @return `true` if all data has been shifted out, or `false` if
	 * shifting is still in progress.
	 */
	bool dataSent();
	/**
	 * Shift out data for all the digital frames in @p context.
	 */
	void process(BelaContext* context);
	/**
	 * Shift out data for frame @p n.
	 */
	void process(BelaContext* context, unsigned int n);
	/**
	 * Set new data bits to be shifted out. Data willl be shifted out during the
	 * subsequent calls to process(), until dataSent() returns `true`.
	 * If the size of the data passed in is larger than
	 * `maxSize` was, the internal buffer holding a copy of the data will
	 * be reallocated, which is not a real-time safe operation.
	 *
	 * @param dataBuf the data to be shifted out.
	 */
	void setData(const std::vector<bool>& dataBuf);
	/**
	 * @copydoc ShiftRegister::setData(const std::vector<bool>&)
	 *
	 * @param length the length of the data.
	 */
	void setData(const bool* dataBuf, unsigned int length);
private:
	Pins pins;
	typedef enum {
		kStart,
		kTransmitting,
		kStop,
		kIdle,
	} State;

	State state = kStop;
	unsigned int currentDataFrame = 0;
	std::vector<bool> data;
	bool pinModeSet = false;
};
