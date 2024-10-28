# Waveform

* NDPluginCV.cpp
	* NDPluginCV::processCallbacks(NDArray *pArray)
	* https://github.com/areaDetector/ADCompVision/blob/master/adcvApp/adcvSrc/NDPluginCV.cpp#L693
	* L693 ->747
	* Convert image to CV
		* status = ndArray2Mat(pArray, img, pArray->dataType, arrayInfo.colorMode);
	* Convert CV to image
		* status = mat2NDArray(img, pScratch);

## ADCore processPlugin for Flatfield Correction
* NDPluginTimeSeries.cpp:368:    doCallbacksGenericPointer(pArrayOut, NDArrayData, numSignals_);
* array of double


