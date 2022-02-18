## Usage examples of the FLDServer

Assuming, you already have the executable, follow these steps:

- Download the OpenFace model from [here](https://drive.google.com/file/d/1Qc7oJJUfDXwt5e76ovTfdsoqa6oiR8JM/view?usp=sharing).
- Extract the zip file and place the content in the same directory where the executable reside.
    - The folder structure should look like this : 
    ```
    AU_predictors/
    model/
    FLDServer
    ```
- Run the server. You should get something like this:
  ``` 
  [0219/000954.550891:INFO:main.cc(21)] Running:  ./FLDServer
  Could not find the HAAR face detector location
  Reading the landmark detector/tracker from: model/main_ceclm_general.txt
  Reading the landmark detector module from: model/cen_general.txt
  Reading the PDM module from: model/pdms/In-the-wild_aligned_PDM_68.txt....Done
  Reading the Triangulations module from: model/tris_68.txt....Done
  Reading the intensity CEN patch experts from: model/patch_experts/cen_patches_0.25_of.dat....Done
  Reading the intensity CEN patch experts from: model/patch_experts/cen_patches_0.35_of.dat....Done
  Reading the intensity CEN patch experts from: model/patch_experts/cen_patches_0.50_of.dat....Done
  Reading the intensity CEN patch experts from: model/patch_experts/cen_patches_1.00_of.dat....Done
  Reading part based module....left_eye_28
  Reading the landmark detector/tracker from: model/model_eye/main_clnf_synth_left.txt
  Reading the landmark detector module from: model/model_eye/clnf_left_synth.txt
  Reading the PDM module from: model/model_eye/pdms/pdm_28_l_eye_3D_closed.txt....Done
  Reading the intensity CCNF patch experts from: model/model_eye/patch_experts/left_ccnf_patches_1.00_synth_lid_.txt....Done
  Reading the intensity CCNF patch experts from: model/model_eye/patch_experts/left_ccnf_patches_1.50_synth_lid_.txt....Done
  Could not find the HAAR face detector location
  Done
  Reading part based module....right_eye_28
  Reading the landmark detector/tracker from: model/model_eye/main_clnf_synth_right.txt
  Reading the landmark detector module from: model/model_eye/clnf_right_synth.txt
  Reading the PDM module from: model/model_eye/pdms/pdm_28_eye_3D_closed.txt....Done
  Reading the intensity CCNF patch experts from: model/model_eye/patch_experts/ccnf_patches_1.00_synth_lid_.txt....Done
  Reading the intensity CCNF patch experts from: model/model_eye/patch_experts/ccnf_patches_1.50_synth_lid_.txt....Done
  Could not find the HAAR face detector location
  Done
  Reading the landmark validation module....Done
  Reading the AU analysis module from: AU_predictors/main_dynamic_svms.txt
  Reading the AU predictors from: AU_predictors/AU_all_best.txt... Done
  Reading the PDM from: AU_predictors/In-the-wild_aligned_PDM_68.txt... Done
  Reading the triangulation from:AU_predictors/tris_68_full.txt... Done
  [0219/000954.764298:INFO:runner.cc(50)] FLDServer started on : 2022-02-18T23:09:54.764Z
  [0219/000954.764306:INFO:runner.cc(51)] Listening on : http://127.0.0.1:34568

  ```
  
### Camera demo
The camera demo located at [camera](./camera) demonstrates, how to fetch video
frames from the camera, encode and send them as base64 strings to the server via an 
http get request. On success, the server returns a json object containing the relevant features. 
