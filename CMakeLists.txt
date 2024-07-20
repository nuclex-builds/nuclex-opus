#!/usr/bin/cmake
cmake_minimum_required (VERSION 3.18)

# -------------------------------------------------------------------------------------------------

project(
	Opus
	VERSION 1.4
	DESCRIPTION "Lossy audio codec that beats any other lossy codec there is"
)

# This sets a bunch of compile flags and defined ${NUCLEX_COMPILER_TAG} to
# say something like linux-gcc-13.2-amd64-debug. You should have this directory
# if you do a full clone of a project that is using this third-party library build.
include("../../build-system/cmake/cplusplus.cmake")

# -------------------------------------------------------------------------------------------------

# TODO: Effin' CMake again. Can't this abomination of a useless build system just die?
#  If downloads the file. It complains about the checksum if it's wrong.
# Then it writes a BAD status into DOWNLOAD_STATUS. And leaves ERROR_MESSAGE empty.
# Completely useless. Weird, custom build language with side effects as the primary
# means of returning results and that probably changes between versions. Not fixing this.
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/build)
	message(STATUS "Build directory not present, downloading sources")

	file(
		DOWNLOAD https://github.com/xiph/opus/archive/refs/tags/v1.4.tar.gz
		${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz
		SHOW_PROGRESS
		EXPECTED_HASH SHA256=7875d49596286055b52439ed42f044bd8ad426aa4cc5aabd96bfe7abb971d5e8
	)
	list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
	if(NOT ${STATUS_CODE} EQUAL 0)
		list(GET DOWNLOAD_STATUS 1 ERROR_MESSAGE)
		file(REMOVE ${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz)
		message(FATAL_ERROR "Could not download Opus: ${ERROR_MESSAGE}")
	endif()

	message(STATUS "Extracting Opus sources")
	file(
		ARCHIVE_EXTRACT
		INPUT ${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz
		DESTINATION ${CMAKE_BINARY_DIR}/extract
	)
	file(
		RENAME
		${CMAKE_BINARY_DIR}/extract/v1.4
		${PROJECT_SOURCE_DIR}/build
	)
endif()

# -------------------------------------------------------------------------------------------------

#configure_file(
#	"${PROJECT_SOURCE_DIR}/config.h"
#	"${PROJECT_SOURCE_DIR}/build/config.h"
#	COPYONLY
#)

# -------------------------------------------------------------------------------------------------

set(
  sourceFiles
  "build/silk/resampler.c"
  "build/silk/bwexpander_32.c"
  "build/silk/HP_variable_cutoff.c"
  "build/silk/decode_pulses.c"
  "build/silk/PLC.c"
  "build/silk/decode_pitch.c"
  "build/silk/inner_prod_aligned.c"
  "build/silk/NLSF2A.c"
  "build/silk/control_audio_bandwidth.c"
  "build/silk/resampler_rom.c"
  "build/silk/sum_sqr_shift.c"
  "build/silk/enc_API.c"
  "build/silk/encode_pulses.c"
  "build/silk/decode_frame.c"
  "build/silk/check_control_input.c"
  "build/silk/control_SNR.c"
  "build/silk/NLSF_VQ_weights_laroia.c"
  "build/silk/fixed/LTP_analysis_filter_FIX.c"
  "build/silk/fixed/process_gains_FIX.c"
  "build/silk/fixed/burg_modified_FIX.c"
  "build/silk/fixed/encode_frame_FIX.c"
  "build/silk/fixed/residual_energy_FIX.c"
  "build/silk/fixed/k2a_FIX.c"
  "build/silk/fixed/residual_energy16_FIX.c"
  "build/silk/fixed/vector_ops_FIX.c"
  "build/silk/fixed/schur64_FIX.c"
  "build/silk/fixed/pitch_analysis_core_FIX.c"
  "build/silk/fixed/LTP_scale_ctrl_FIX.c"
  "build/silk/fixed/arm/warped_autocorrelation_FIX_neon_intr.c"
  "build/silk/fixed/apply_sine_window_FIX.c"
  "build/silk/fixed/autocorr_FIX.c"
  "build/silk/fixed/corrMatrix_FIX.c"
  "build/silk/fixed/x86/vector_ops_FIX_sse4_1.c"
  "build/silk/fixed/x86/burg_modified_FIX_sse4_1.c"
  "build/silk/fixed/noise_shape_analysis_FIX.c"
  "build/silk/fixed/warped_autocorrelation_FIX.c"
  "build/silk/fixed/find_pitch_lags_FIX.c"
  "build/silk/fixed/find_LTP_FIX.c"
  "build/silk/fixed/schur_FIX.c"
  "build/silk/fixed/find_pred_coefs_FIX.c"
  "build/silk/fixed/find_LPC_FIX.c"
  "build/silk/fixed/regularize_correlations_FIX.c"
  "build/silk/fixed/k2a_Q16_FIX.c"
  "build/silk/resampler_private_AR2.c"
  "build/silk/stereo_decode_pred.c"
  "build/silk/interpolate.c"
  "build/silk/tables_NLSF_CB_NB_MB.c"
  "build/silk/tables_NLSF_CB_WB.c"
  "build/silk/stereo_find_predictor.c"
  "build/silk/resampler_private_down_FIR.c"
  "build/silk/tables_pitch_lag.c"
  "build/silk/VAD.c"
  "build/silk/NLSF_stabilize.c"
  "build/silk/NLSF_unpack.c"
  "build/silk/decode_core.c"
  "build/silk/pitch_est_tables.c"
  "build/silk/control_codec.c"
  "build/silk/float/LTP_analysis_filter_FLP.c"
  "build/silk/float/process_gains_FLP.c"
  "build/silk/float/pitch_analysis_core_FLP.c"
  "build/silk/float/scale_copy_vector_FLP.c"
  "build/silk/float/LTP_scale_ctrl_FLP.c"
  "build/silk/float/warped_autocorrelation_FLP.c"
  "build/silk/float/energy_FLP.c"
  "build/silk/float/scale_vector_FLP.c"
  "build/silk/float/find_LPC_FLP.c"
  "build/silk/float/sort_FLP.c"
  "build/silk/float/x86/inner_product_FLP_avx2.c"
  "build/silk/float/inner_product_FLP.c"
  "build/silk/float/k2a_FLP.c"
  "build/silk/float/autocorrelation_FLP.c"
  "build/silk/float/find_pred_coefs_FLP.c"
  "build/silk/float/wrappers_FLP.c"
  "build/silk/float/find_pitch_lags_FLP.c"
  "build/silk/float/LPC_inv_pred_gain_FLP.c"
  "build/silk/float/noise_shape_analysis_FLP.c"
  "build/silk/float/burg_modified_FLP.c"
  "build/silk/float/schur_FLP.c"
  "build/silk/float/encode_frame_FLP.c"
  "build/silk/float/find_LTP_FLP.c"
  "build/silk/float/apply_sine_window_FLP.c"
  "build/silk/float/LPC_analysis_filter_FLP.c"
  "build/silk/float/residual_energy_FLP.c"
  "build/silk/float/regularize_correlations_FLP.c"
  "build/silk/float/corrMatrix_FLP.c"
  "build/silk/float/bwexpander_FLP.c"
  "build/silk/VQ_WMat_EC.c"
  "build/silk/resampler_down2.c"
  "build/silk/sigm_Q15.c"
  "build/silk/NLSF_del_dec_quant.c"
  "build/silk/dec_API.c"
  "build/silk/NSQ.c"
  "build/silk/gain_quant.c"
  "build/silk/decode_indices.c"
  "build/silk/bwexpander.c"
  "build/silk/quant_LTP_gains.c"
  "build/silk/arm/biquad_alt_neon_intr.c"
  "build/silk/arm/LPC_inv_pred_gain_neon_intr.c"
  "build/silk/arm/arm_silk_map.c"
  "build/silk/arm/NSQ_del_dec_neon_intr.c"
  "build/silk/arm/NSQ_neon.c"
  "build/silk/A2NLSF.c"
  "build/silk/tables_other.c"
  "build/silk/LPC_inv_pred_gain.c"
  "build/silk/biquad_alt.c"
  "build/silk/init_encoder.c"
  "build/silk/LPC_fit.c"
  "build/silk/sort.c"
  "build/silk/x86/NSQ_del_dec_avx2.c"
  "build/silk/x86/NSQ_del_dec_sse4_1.c"
  "build/silk/x86/VQ_WMat_EC_sse4_1.c"
  "build/silk/x86/VAD_sse4_1.c"
  "build/silk/x86/x86_silk_map.c"
  "build/silk/x86/NSQ_sse4_1.c"
  "build/silk/CNG.c"
  "build/silk/shell_coder.c"
  "build/silk/resampler_down2_3.c"
  "build/silk/LPC_analysis_filter.c"
  "build/silk/LP_variable_cutoff.c"
  "build/silk/code_signs.c"
  "build/silk/tests/test_unit_LPC_inv_pred_gain.c"
  "build/silk/tables_gain.c"
  "build/silk/tables_pulses_per_block.c"
  "build/silk/encode_indices.c"
  "build/silk/stereo_encode_pred.c"
  "build/silk/stereo_LR_to_MS.c"
  "build/silk/NLSF_decode.c"
  "build/silk/lin2log.c"
  "build/silk/stereo_MS_to_LR.c"
  "build/silk/resampler_private_IIR_FIR.c"
  "build/silk/stereo_quant_pred.c"
  "build/silk/debug.c"
  "build/silk/NLSF_VQ.c"
  "build/silk/decoder_set_fs.c"
  "build/silk/NSQ_del_dec.c"
  "build/silk/NLSF_encode.c"
  "build/silk/resampler_private_up2_HQ.c"
  "build/silk/process_NLSFs.c"
  "build/silk/decode_parameters.c"
  "build/silk/log2lin.c"
  "build/silk/ana_filt_bank_1.c"
  "build/silk/init_decoder.c"
  "build/silk/tables_LTP.c"
  "build/silk/table_LSF_cos.c"
  "build/dnn/parse_lpcnet_weights.c"
  "build/dnn/lpcnet_enc.c"
  "build/dnn/fwgan.c"
  "build/dnn/lpcnet_plc.c"
  "build/dnn/dred_coding.c"
  "build/dnn/test_vec.c"
  "build/dnn/dump_lpcnet_tables.c"
  "build/dnn/nnet.c"
  "build/dnn/dred_rdovae_dec.c"
  "build/dnn/lossgen_demo.c"
  "build/dnn/fargan.c"
  "build/dnn/write_lpcnet_weights.c"
  "build/dnn/fargan_demo.c"
  "build/dnn/dred_rdovae_enc.c"
  "build/dnn/arm/nnet_neon.c"
  "build/dnn/arm/nnet_dotprod.c"
  "build/dnn/arm/arm_dnn_map.c"
  "build/dnn/kiss99.c"
  "build/dnn/lossgen.c"
  "build/dnn/osce.c"
  "build/dnn/x86/x86_dnn_map.c"
  "build/dnn/x86/nnet_avx2.c"
  "build/dnn/x86/nnet_sse2.c"
  "build/dnn/x86/nnet_sse4_1.c"
  "build/dnn/dred_decoder.c"
  "build/dnn/burg.c"
  "build/dnn/nndsp.c"
  "build/dnn/nnet_default.c"
  "build/dnn/adaconvtest.c"
  "build/dnn/osce_features.c"
  "build/dnn/pitchdnn.c"
  "build/dnn/freq.c"
  "build/dnn/lpcnet_tables.c"
  "build/dnn/dump_data.c"
  "build/dnn/dred_encoder.c"
  "build/dnn/lpcnet.c"
  "build/dnn/torch/rdovae/packets/fec_packets.c"
  "build/dnn/training_tf2/fec_packets.c"
  "build/tests/opus_encode_regressions.c"
  "build/tests/test_opus_encode.c"
  "build/tests/test_opus_projection.c"
  "build/tests/opus_decode_fuzzer.c"
  "build/tests/test_opus_padding.c"
  "build/tests/test_opus_extensions.c"
  "build/tests/test_opus_dred.c"
  "build/tests/test_opus_api.c"
  "build/tests/test_opus_decode.c"
  "build/celt/dump_modes/dump_modes.c"
  "build/celt/dump_modes/dump_modes_arm_ne10.c"
  "build/celt/entenc.c"
  "build/celt/quant_bands.c"
  "build/celt/kiss_fft.c"
  "build/celt/celt_decoder.c"
  "build/celt/celt_encoder.c"
  "build/celt/arm/celt_fft_ne10.c"
  "build/celt/arm/celt_neon_intr.c"
  "build/celt/arm/celt_mdct_ne10.c"
  "build/celt/arm/arm_celt_map.c"
  "build/celt/arm/pitch_neon_intr.c"
  "build/celt/arm/armcpu.c"
  "build/celt/celt.c"
  "build/celt/mathops.c"
  "build/celt/cwrs.c"
  "build/celt/laplace.c"
  "build/celt/entdec.c"
  "build/celt/x86/pitch_sse2.c"
  "build/celt/x86/x86_celt_map.c"
  "build/celt/x86/pitch_sse.c"
  "build/celt/x86/x86cpu.c"
  "build/celt/x86/celt_lpc_sse4_1.c"
  "build/celt/x86/vq_sse2.c"
  "build/celt/x86/pitch_avx.c"
  "build/celt/x86/pitch_sse4_1.c"
  "build/celt/opus_custom_demo.c"
  "build/celt/pitch.c"
  "build/celt/tests/test_unit_cwrs32.c"
  "build/celt/tests/test_unit_mathops.c"
  "build/celt/tests/test_unit_dft.c"
  "build/celt/tests/test_unit_laplace.c"
  "build/celt/tests/test_unit_mdct.c"
  "build/celt/tests/test_unit_entropy.c"
  "build/celt/tests/test_unit_types.c"
  "build/celt/tests/test_unit_rotation.c"
  "build/celt/bands.c"
  "build/celt/vq.c"
  "build/celt/rate.c"
  "build/celt/entcode.c"
  "build/celt/mdct.c"
  "build/celt/celt_lpc.c"
  "build/celt/modes.c"
  "build/src/mapping_matrix.c"
  "build/src/opus_encoder.c"
  "build/src/opus_multistream.c"
  "build/src/opus_multistream_decoder.c"
  "build/src/mlp_data.c"
  "build/src/opus_decoder.c"
  "build/src/opus_demo.c"
  "build/src/repacketizer.c"
  "build/src/repacketizer_demo.c"
  "build/src/opus_multistream_encoder.c"
  "build/src/opus_projection_encoder.c"
  "build/src/analysis.c"
  "build/src/mlp.c"
  "build/src/opus_projection_decoder.c"
  "build/src/opus.c"
  "build/src/opus_compare.c"
  "build/src/extensions.c"
  "build/doc/customdoxygen.css"
  "build/doc/opus_in_isobmff.css"
  "build/doc/trivial_example.c"
  "build/cmake/RunTest.cmake"
  "build/cmake/config.h.cmake.in"
  "build/cmake/cpu_info_by_asm.c"
  "build/cmake/cpu_info_by_c.c"
  "build/cmake/vla.c"
)
set(
  headerFiles
"build/silk/resampler_structs.h"
  "build/silk/main.h"
  "build/silk/define.h"
  "build/silk/pitch_est_defines.h"
  "build/silk/macros.h"
  "build/silk/fixed/structs_FIX.h"
  "build/silk/fixed/arm/warped_autocorrelation_FIX_arm.h"
  "build/silk/fixed/main_FIX.h"
  "build/silk/fixed/mips/prefilter_FIX_mipsr1.h"
  "build/silk/fixed/mips/noise_shape_analysis_FIX_mipsr1.h"
  "build/silk/fixed/mips/warped_autocorrelation_FIX_mipsr1.h"
  "build/silk/errors.h"
  "build/silk/SigProc_FIX.h"
  "build/silk/debug.h"
  "build/silk/PLC.h"
  "build/silk/resampler_rom.h"
  "build/silk/float/SigProc_FLP.h"
  "build/silk/float/main_FLP.h"
  "build/silk/float/structs_FLP.h"
  "build/silk/NSQ.h"
  "build/silk/arm/macros_armv5e.h"
  "build/silk/arm/macros_armv4.h"
  "build/silk/arm/SigProc_FIX_armv4.h"
  "build/silk/arm/SigProc_FIX_armv5e.h"
  "build/silk/arm/macros_arm64.h"
  "build/silk/arm/LPC_inv_pred_gain_arm.h"
  "build/silk/arm/NSQ_del_dec_arm.h"
  "build/silk/arm/NSQ_neon.h"
  "build/silk/arm/biquad_alt_arm.h"
  "build/silk/MacroCount.h"
  "build/silk/MacroDebug.h"
  "build/silk/mips/NSQ_del_dec_mipsr1.h"
  "build/silk/mips/sigproc_fix_mipsr1.h"
  "build/silk/mips/macros_mipsr1.h"
  "build/silk/x86/SigProc_FIX_sse.h"
  "build/silk/x86/main_sse.h"
  "build/silk/resampler_private.h"
  "build/silk/typedef.h"
  "build/silk/structs.h"
  "build/silk/API.h"
  "build/silk/control.h"
  "build/silk/Inlines.h"
  "build/silk/tables.h"
  "build/silk/tuning_parameters.h"
  "build/dnn/dred_config.h"
  "build/dnn/fwgan.h"
  "build/dnn/vec.h"
  "build/dnn/dred_encoder.h"
  "build/dnn/lpcnet_private.h"
  "build/dnn/osce.h"
  "build/dnn/kiss99.h"
  "build/dnn/lossgen.h"
  "build/dnn/tansig_table.h"
  "build/dnn/burg.h"
  "build/dnn/osce_structs.h"
  "build/dnn/osce_config.h"
  "build/dnn/arm/dnn_arm.h"
  "build/dnn/dred_coding.h"
  "build/dnn/nnet.h"
  "build/dnn/common.h"
  "build/dnn/dred_decoder.h"
  "build/dnn/x86/dnn_x86.h"
  "build/dnn/nndsp.h"
  "build/dnn/dred_rdovae.h"
  "build/dnn/osce_features.h"
  "build/dnn/nnet_arch.h"
  "build/dnn/lpcnet.h"
  "build/dnn/vec_avx.h"
  "build/dnn/freq.h"
  "build/dnn/dred_rdovae_dec.h"
  "build/dnn/vec_neon.h"
  "build/dnn/fargan.h"
  "build/dnn/pitchdnn.h"
  "build/dnn/dred_rdovae_enc.h"
  "build/dnn/torch/rdovae/packets/fec_packets.h"
  "build/dnn/training_tf2/fec_packets.h"
  "build/tests/test_opus_common.h"
  "build/celt/dump_modes/dump_modes_arch.h"
  "build/celt/entenc.h"
  "build/celt/static_modes_fixed_arm_ne10.h"
  "build/celt/static_modes_float_arm_ne10.h"
  "build/celt/entdec.h"
  "build/celt/static_modes_float.h"
  "build/celt/mathops.h"
  "build/celt/cwrs.h"
  "build/celt/modes.h"
  "build/celt/celt.h"
  "build/celt/bands.h"
  "build/celt/arm/mdct_arm.h"
  "build/celt/arm/fixed_armv4.h"
  "build/celt/arm/fixed_armv5e.h"
  "build/celt/arm/pitch_arm.h"
  "build/celt/arm/armcpu.h"
  "build/celt/arm/fft_arm.h"
  "build/celt/arm/kiss_fft_armv5e.h"
  "build/celt/arm/kiss_fft_armv4.h"
  "build/celt/arm/fixed_arm64.h"
  "build/celt/entcode.h"
  "build/celt/kiss_fft.h"
  "build/celt/stack_alloc.h"
  "build/celt/cpu_support.h"
  "build/celt/laplace.h"
  "build/celt/rate.h"
  "build/celt/vq.h"
  "build/celt/mips/vq_mipsr1.h"
  "build/celt/mips/mdct_mipsr1.h"
  "build/celt/mips/kiss_fft_mipsr1.h"
  "build/celt/mips/pitch_mipsr1.h"
  "build/celt/mips/celt_mipsr1.h"
  "build/celt/mips/fixed_generic_mipsr1.h"
  "build/celt/x86/x86cpu.h"
  "build/celt/x86/x86_arch_macros.h"
  "build/celt/x86/celt_lpc_sse.h"
  "build/celt/x86/pitch_sse.h"
  "build/celt/x86/vq_sse.h"
  "build/celt/ecintrin.h"
  "build/celt/mdct.h"
  "build/celt/mfrngcod.h"
  "build/celt/arch.h"
  "build/celt/fixed_generic.h"
  "build/celt/fixed_c6x.h"
  "build/celt/fixed_debug.h"
  "build/celt/fixed_c5x.h"
  "build/celt/_kiss_fft_guts.h"
  "build/celt/quant_bands.h"
  "build/celt/static_modes_fixed.h"
  "build/celt/float_cast.h"
  "build/celt/os_support.h"
  "build/celt/celt_lpc.h"
  "build/celt/pitch.h"
  "build/src/mlp.h"
  "build/src/analysis.h"
  "build/src/mapping_matrix.h"
  "build/src/tansig_table.h"
  "build/src/opus_private.h"
  "build/doc/header.html"
  "build/doc/opus_in_isobmff.html"
  "build/doc/footer.html"
  "build/cmake/config.h.cmake.in"
  "build/include/opus_projection.h"
  "build/include/opus_custom.h"
  "build/include/opus_defines.h"
  "build/include/opus_multistream.h"
  "build/include/opus_types.h"
  "build/include/opus.h"
)

# -------------------------------------------------------------------------------------------------

add_library(Opus STATIC)
add_library(Opus::Static ALIAS Opus)

target_compile_definitions(
	Opus
	PRIVATE OPUS_EXPORTS
)

target_include_directories(
	Opus
	PUBLIC "build/lib"
)

target_sources(
	Opus
	PUBLIC ${headerFiles}
	PRIVATE ${sourceFiles}
)

set_target_properties(Opus PROPERTIES PREFIX "")
set_target_properties(Opus PROPERTIES OUTPUT_NAME "opus")

# -------------------------------------------------------------------------------------------------

install(
	TARGETS Opus
	ARCHIVE	DESTINATION ${PROJECT_SOURCE_DIR}/bin/${NUCLEX_COMPILER_TAG}
)

# -------------------------------------------------------------------------------------------------

file(
	WRITE "${PROJECT_SOURCE_DIR}/OpusConfig.cmake"
	"#!/usr/bin/cmake

# Configuration to include Opus in a CMake-based project. If you want to
# reference Opus as an externally compiled static library, do this:
#
#    set(Opus_DIR \"../ThirdParty/opus\")
#    find_package(Opus REQUIRED CONFIG)
#
#    target_link_libraries(
#        MyAwesomeProject
#        PRIVATE Opus::Static
#    )
#
# Alternatively, if you want to build Opus together with your project,
# use the normal CMakeLists.txt with CMake's add_subdirectory() command:
#
#    add_subdirectory(
#        \"\${PROJECT_SOURCE_DIR}/../ThirdParty/opus\"
#        \"\${CMAKE_BINARY_DIR}/opus\"
#    )
#
#    target_link_libraries(
#        MyAwesomeProject
#        PRIVATE Opus
#    )
#

# -------------------------------------------------------------------------------------------------

if(NOT DEFINED NUCLEX_COMPILER_TAG)
	message(
		FATAL_ERROR
		\"NUCLEX_COMPILER_TAG not defined! Include cplusplus.cmake before importing this package \\
		in order to generate a tag identifying the platform/compiler/architecture/variant!\"
	)
endif()

# -------------------------------------------------------------------------------------------------

if(NOT EXISTS \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}\")
	# TODO: Warn and link release build when compiling in debug mode
	# TODO: Warn and link build for older compiler version if found
	message(
		FATAL_ERROR
		\"Directory '\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}' not found. \\
		Please either build and install this project before importing it via \\
		find_package() or use this project's main CMakeFiles.txt via add_subdirectory()!\"
	)
endif()

# -------------------------------------------------------------------------------------------------

add_library(Opus::Static STATIC IMPORTED)

# This may cause warnings on recent GCC versions (10.0.0+?) with LTO because GCC detects
# that the headers used during build (residing in build/) are not the same used when
# linking the library (copies resising in Include/).
#
# CMake doesn't run the install step during build, so the only way to get the headers
# in place before building would be by copying them rather than installing them.
set_target_properties(
	Opus::Static PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES \"\${CMAKE_CURRENT_LIST_DIR}/Include\"
	IMPORTED_LINK_INTERFACE_LANGUAGES \"C\"
)

if(WIN32)
	set_target_properties(
		Opus::Static PROPERTIES
		IMPORTED_LOCATION \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}/opus.lib\"
	)
else()
	set_target_properties(
		Opus::Static PROPERTIES
		IMPORTED_LOCATION \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}/opus.a\"
	)
endif()

message(STATUS \"Imported Opus targets with binaries in '\${CMAKE_CURRENT_LIST_DIR}'\")"
)

# -------------------------------------------------------------------------------------------------
