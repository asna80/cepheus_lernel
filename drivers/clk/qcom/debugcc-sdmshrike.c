/*
 * Copyright (c) 2019, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#define pr_fmt(fmt) "clk: %s: " fmt, __func__

#include <linux/platform_device.h>
#include <linux/module.h>
#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/mfd/syscon.h>

#include "clk-debug.h"

static struct measure_clk_data debug_mux_priv = {
	.ctl_reg = 0x62038,
	.status_reg = 0x6203C,
	.xo_div4_cbcr = 0x43008,
};

static const char *const debug_mux_parent_names[] = {
	"measure_only_mccc_clk",
	"measure_only_cnoc_clk",
	"measure_only_ipa_2x_clk",
	"measure_only_snoc_clk",
	"l3_clk",
	"pwrcl_clk",
	"perfcl_clk",
	"cam_cc_bps_ahb_clk",
	"cam_cc_bps_areg_clk",
	"cam_cc_bps_axi_clk",
	"cam_cc_bps_clk",
	"cam_cc_camnoc_axi_clk",
	"cam_cc_cci_0_clk",
	"cam_cc_cci_1_clk",
	"cam_cc_cci_2_clk",
	"cam_cc_cci_3_clk",
	"cam_cc_core_ahb_clk",
	"cam_cc_cpas_ahb_clk",
	"cam_cc_csi0phytimer_clk",
	"cam_cc_csi1phytimer_clk",
	"cam_cc_csi2phytimer_clk",
	"cam_cc_csi3phytimer_clk",
	"cam_cc_csiphy0_clk",
	"cam_cc_csiphy1_clk",
	"cam_cc_csiphy2_clk",
	"cam_cc_csiphy3_clk",
	"cam_cc_fd_core_clk",
	"cam_cc_fd_core_uar_clk",
	"cam_cc_icp_ahb_clk",
	"cam_cc_icp_clk",
	"cam_cc_ife_0_axi_clk",
	"cam_cc_ife_0_clk",
	"cam_cc_ife_0_cphy_rx_clk",
	"cam_cc_ife_0_csid_clk",
	"cam_cc_ife_0_dsp_clk",
	"cam_cc_ife_1_axi_clk",
	"cam_cc_ife_1_clk",
	"cam_cc_ife_1_cphy_rx_clk",
	"cam_cc_ife_1_csid_clk",
	"cam_cc_ife_1_dsp_clk",
	"cam_cc_ife_2_axi_clk",
	"cam_cc_ife_2_clk",
	"cam_cc_ife_2_cphy_rx_clk",
	"cam_cc_ife_2_csid_clk",
	"cam_cc_ife_2_dsp_clk",
	"cam_cc_ife_3_axi_clk",
	"cam_cc_ife_3_clk",
	"cam_cc_ife_3_cphy_rx_clk",
	"cam_cc_ife_3_csid_clk",
	"cam_cc_ife_3_dsp_clk",
	"cam_cc_ife_lite_0_clk",
	"cam_cc_ife_lite_0_cphy_rx_clk",
	"cam_cc_ife_lite_0_csid_clk",
	"cam_cc_ife_lite_1_clk",
	"cam_cc_ife_lite_1_cphy_rx_clk",
	"cam_cc_ife_lite_1_csid_clk",
	"cam_cc_ife_lite_2_clk",
	"cam_cc_ife_lite_2_cphy_rx_clk",
	"cam_cc_ife_lite_2_csid_clk",
	"cam_cc_ife_lite_3_clk",
	"cam_cc_ife_lite_3_cphy_rx_clk",
	"cam_cc_ife_lite_3_csid_clk",
	"cam_cc_ipe_0_ahb_clk",
	"cam_cc_ipe_0_areg_clk",
	"cam_cc_ipe_0_axi_clk",
	"cam_cc_ipe_0_clk",
	"cam_cc_ipe_1_ahb_clk",
	"cam_cc_ipe_1_areg_clk",
	"cam_cc_ipe_1_axi_clk",
	"cam_cc_ipe_1_clk",
	"cam_cc_jpeg_clk",
	"cam_cc_lrme_clk",
	"cam_cc_mclk0_clk",
	"cam_cc_mclk1_clk",
	"cam_cc_mclk2_clk",
	"cam_cc_mclk3_clk",
	"cam_cc_mclk4_clk",
	"cam_cc_mclk5_clk",
	"cam_cc_mclk6_clk",
	"cam_cc_mclk7_clk",
	"disp_cc_mdss_ahb_clk",
	"disp_cc_mdss_byte0_clk",
	"disp_cc_mdss_byte0_intf_clk",
	"disp_cc_mdss_byte1_clk",
	"disp_cc_mdss_byte1_intf_clk",
	"disp_cc_mdss_dp_crypto1_clk",
	"disp_cc_mdss_dp_crypto_clk",
	"disp_cc_mdss_dp_link1_clk",
	"disp_cc_mdss_dp_link1_intf_clk",
	"disp_cc_mdss_dp_link_clk",
	"disp_cc_mdss_dp_link_intf_clk",
	"disp_cc_mdss_dp_pixel1_clk",
	"disp_cc_mdss_dp_pixel2_clk",
	"disp_cc_mdss_dp_pixel_clk",
	"disp_cc_mdss_edp_link_clk",
	"disp_cc_mdss_edp_link_intf_clk",
	"disp_cc_mdss_edp_pixel_clk",
	"disp_cc_mdss_mdp_clk",
	"disp_cc_mdss_mdp_lut_clk",
	"disp_cc_mdss_non_gdsc_ahb_clk",
	"disp_cc_mdss_pclk0_clk",
	"disp_cc_mdss_pclk1_clk",
	"disp_cc_mdss_rot_clk",
	"disp_cc_mdss_rscc_ahb_clk",
	"gcc_aggre_ufs_card_2_axi_clk",
	"gcc_aggre_ufs_card_axi_clk",
	"gcc_aggre_ufs_phy_axi_clk",
	"gcc_aggre_usb3_mp_axi_clk",
	"gcc_aggre_usb3_prim_axi_clk",
	"gcc_aggre_usb3_sec_axi_clk",
	"gcc_cfg_noc_usb3_mp_axi_clk",
	"gcc_cfg_noc_usb3_prim_axi_clk",
	"gcc_cfg_noc_usb3_sec_axi_clk",
	"gcc_cpuss_ahb_clk",
	"gcc_emac_ptp_clk",
	"gcc_emac_rgmii_clk",
	"gcc_gp1_clk",
	"gcc_gp2_clk",
	"gcc_gp3_clk",
	"gcc_gp4_clk",
	"gcc_gp5_clk",
	"gcc_gpu_gpll0_clk_src",
	"gcc_gpu_gpll0_div_clk_src",
	"gcc_npu_axi_clk",
	"gcc_npu_gpll0_clk_src",
	"gcc_npu_gpll0_div_clk_src",
	"gcc_pcie0_phy_refgen_clk",
	"gcc_pcie1_phy_refgen_clk",
	"gcc_pcie2_phy_refgen_clk",
	"gcc_pcie3_phy_refgen_clk",
	"gcc_pcie_0_aux_clk",
	"gcc_pcie_1_aux_clk",
	"gcc_pcie_2_aux_clk",
	"gcc_pcie_2_pipe_clk",
	"gcc_pcie_2_slv_axi_clk",
	"gcc_pcie_2_slv_q2a_axi_clk",
	"gcc_pcie_3_aux_clk",
	"gcc_pcie_3_cfg_ahb_clk",
	"gcc_pcie_3_mstr_axi_clk",
	"gcc_pcie_3_pipe_clk",
	"gcc_pcie_3_slv_axi_clk",
	"gcc_pcie_3_slv_q2a_axi_clk",
	"gcc_pcie_phy_aux_clk",
	"gcc_pdm2_clk",
	"gcc_pdm_ahb_clk",
	"gcc_prng_ahb_clk",
	"gcc_qspi_1_cnoc_periph_ahb_clk",
	"gcc_qspi_1_core_clk",
	"gcc_qspi_cnoc_periph_ahb_clk",
	"gcc_qspi_core_clk",
	"gcc_qupv3_wrap0_core_2x_clk",
	"gcc_qupv3_wrap0_core_clk",
	"gcc_qupv3_wrap0_s0_clk",
	"gcc_qupv3_wrap0_s1_clk",
	"gcc_qupv3_wrap0_s2_clk",
	"gcc_qupv3_wrap0_s3_clk",
	"gcc_qupv3_wrap0_s4_clk",
	"gcc_qupv3_wrap0_s5_clk",
	"gcc_qupv3_wrap0_s6_clk",
	"gcc_qupv3_wrap0_s7_clk",
	"gcc_qupv3_wrap1_core_2x_clk",
	"gcc_qupv3_wrap1_core_clk",
	"gcc_qupv3_wrap1_s0_clk",
	"gcc_qupv3_wrap1_s1_clk",
	"gcc_qupv3_wrap1_s2_clk",
	"gcc_qupv3_wrap1_s3_clk",
	"gcc_qupv3_wrap1_s4_clk",
	"gcc_qupv3_wrap1_s5_clk",
	"gcc_qupv3_wrap2_core_2x_clk",
	"gcc_qupv3_wrap2_core_clk",
	"gcc_qupv3_wrap2_s0_clk",
	"gcc_qupv3_wrap2_s1_clk",
	"gcc_qupv3_wrap2_s2_clk",
	"gcc_qupv3_wrap2_s3_clk",
	"gcc_qupv3_wrap2_s4_clk",
	"gcc_qupv3_wrap2_s5_clk",
	"gcc_sdcc2_ahb_clk",
	"gcc_sdcc2_apps_clk",
	"gcc_sdcc4_ahb_clk",
	"gcc_sdcc4_apps_clk",
	"gcc_sys_noc_cpuss_ahb_clk",
	"gcc_tsif_ahb_clk",
	"gcc_tsif_ref_clk",
	"gcc_ufs_card_2_ahb_clk",
	"gcc_ufs_card_2_axi_clk",
	"gcc_ufs_card_2_ice_core_clk",
	"gcc_ufs_card_2_phy_aux_clk",
	"gcc_ufs_card_2_rx_symbol_0_clk",
	"gcc_ufs_card_2_rx_symbol_1_clk",
	"gcc_ufs_card_2_tx_symbol_0_clk",
	"gcc_ufs_card_2_unipro_core_clk",
	"gcc_ufs_card_ahb_clk",
	"gcc_ufs_card_axi_clk",
	"gcc_ufs_card_ice_core_clk",
	"gcc_ufs_card_rx_symbol_0_clk",
	"gcc_ufs_card_rx_symbol_1_clk",
	"gcc_ufs_card_tx_symbol_0_clk",
	"gcc_ufs_card_unipro_core_clk",
	"gcc_ufs_phy_ahb_clk",
	"gcc_ufs_phy_axi_clk",
	"gcc_ufs_phy_ice_core_clk",
	"gcc_ufs_phy_rx_symbol_0_clk",
	"gcc_ufs_phy_rx_symbol_1_clk",
	"gcc_ufs_phy_tx_symbol_0_clk",
	"gcc_ufs_phy_unipro_core_clk",
	"gcc_usb30_mp_master_clk",
	"gcc_usb30_mp_mock_utmi_clk",
	"gcc_usb30_prim_master_clk",
	"gcc_usb30_prim_mock_utmi_clk",
	"gcc_usb30_sec_master_clk",
	"gcc_usb30_sec_mock_utmi_clk",
	"gcc_usb3_mp_phy_com_aux_clk",
	"gcc_usb3_mp_phy_pipe_0_clk",
	"gcc_usb3_mp_phy_pipe_1_clk",
	"gcc_usb3_prim_phy_pipe_clk",
	"gcc_usb3_sec_phy_pipe_clk",
	"gcc_video_ahb_clk",
	"gcc_video_axi0_clk",
	"gcc_video_axi1_clk",
	"gcc_video_axic_clk",
	"gpu_cc_ahb_clk",
	"gpu_cc_cx_apb_clk",
	"gpu_cc_cx_gmu_clk",
	"gpu_cc_cx_qdss_at_clk",
	"gpu_cc_cx_qdss_trig_clk",
	"gpu_cc_cx_snoc_dvm_clk",
	"gpu_cc_gx_gmu_clk",
	"gpu_cc_gx_vsense_clk",
	"npu_cc_armwic_core_clk",
	"npu_cc_cal_dp_cdc_clk",
	"npu_cc_cal_dp_clk",
	"npu_cc_comp_noc_axi_clk",
	"npu_cc_conf_noc_ahb_clk",
	"npu_cc_npu_core_atb_clk",
	"npu_cc_npu_core_clk",
	"npu_cc_npu_core_cti_clk",
	"npu_cc_npu_cpc_clk",
	"npu_cc_perf_cnt_clk",
	"video_cc_iris_ahb_clk",
	"video_cc_mvs0_core_clk",
	"video_cc_mvs1_core_clk",
	"video_cc_mvsc_core_clk",
};

static struct clk_debug_mux gcc_debug_mux = {
	.priv = &debug_mux_priv,
	.debug_offset = 0x62000,
	.post_div_offset = 0x62004,
	.cbcr_offset = 0x62008,
	.src_sel_mask = 0x3FF,
	.src_sel_shift = 0,
	.post_div_mask = 0xF,
	.post_div_shift = 0,
	.period_offset = 0x50,
	MUX_SRC_LIST(
		{ "measure_only_mccc_clk", 0xC2, 2, MC_CC,
			0xC2, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "measure_only_cnoc_clk", 0x15, 2, GCC,
			0x15, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "measure_only_ipa_2x_clk", 0x128, 2, GCC,
			0x128, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "measure_only_snoc_clk", 0x7, 2, GCC,
			0x7, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "l3_clk", 0xD6, 2, CPU_CC,
			0x46, 0x7F, 4, 0xF, 11, 1, 0x0, 0x0, U32_MAX, 16 },
		{ "pwrcl_clk", 0xD6, 2, CPU_CC,
			0x44, 0x7F, 4, 0xF, 11, 1, 0x0, 0x0, U32_MAX, 16 },
		{ "perfcl_clk", 0xD6, 2, CPU_CC,
			0x45, 0x7F, 4, 0xF, 11, 1, 0x0, 0x0, U32_MAX, 16 },
		{ "cam_cc_bps_ahb_clk", 0x46, 2, CAM_CC,
			0xE, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_bps_areg_clk", 0x46, 2, CAM_CC,
			0xD, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_bps_axi_clk", 0x46, 2, CAM_CC,
			0xC, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_bps_clk", 0x46, 2, CAM_CC,
			0xB, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_camnoc_axi_clk", 0x46, 2, CAM_CC,
			0x27, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_cci_0_clk", 0x46, 2, CAM_CC,
			0x2A, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_cci_1_clk", 0x46, 2, CAM_CC,
			0x3B, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_cci_2_clk", 0x46, 2, CAM_CC,
			0x5B, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_cci_3_clk", 0x46, 2, CAM_CC,
			0x5C, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_core_ahb_clk", 0x46, 2, CAM_CC,
			0x2E, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_cpas_ahb_clk", 0x46, 2, CAM_CC,
			0x2C, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csi0phytimer_clk", 0x46, 2, CAM_CC,
			0x5, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csi1phytimer_clk", 0x46, 2, CAM_CC,
			0x7, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csi2phytimer_clk", 0x46, 2, CAM_CC,
			0x9, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csi3phytimer_clk", 0x46, 2, CAM_CC,
			0x35, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csiphy0_clk", 0x46, 2, CAM_CC,
			0x6, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csiphy1_clk", 0x46, 2, CAM_CC,
			0x8, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csiphy2_clk", 0x46, 2, CAM_CC,
			0xA, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_csiphy3_clk", 0x46, 2, CAM_CC,
			0x36, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_fd_core_clk", 0x46, 2, CAM_CC,
			0x28, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_fd_core_uar_clk", 0x46, 2, CAM_CC,
			0x29, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_icp_ahb_clk", 0x46, 2, CAM_CC,
			0x37, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_icp_clk", 0x46, 2, CAM_CC,
			0x26, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_0_axi_clk", 0x46, 2, CAM_CC,
			0x1B, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_0_clk", 0x46, 2, CAM_CC,
			0x17, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_0_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x1A, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_0_csid_clk", 0x46, 2, CAM_CC,
			0x19, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_0_dsp_clk", 0x46, 2, CAM_CC,
			0x18, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_1_axi_clk", 0x46, 2, CAM_CC,
			0x21, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_1_clk", 0x46, 2, CAM_CC,
			0x1D, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_1_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x20, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_1_csid_clk", 0x46, 2, CAM_CC,
			0x1F, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_1_dsp_clk", 0x46, 2, CAM_CC,
			0x1E, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_2_axi_clk", 0x46, 2, CAM_CC,
			0x4A, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_2_clk", 0x46, 2, CAM_CC,
			0x44, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_2_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x49, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_2_csid_clk", 0x46, 2, CAM_CC,
			0x47, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_2_dsp_clk", 0x46, 2, CAM_CC,
			0x46, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_3_axi_clk", 0x46, 2, CAM_CC,
			0x51, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_3_clk", 0x46, 2, CAM_CC,
			0x4B, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_3_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x50, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_3_csid_clk", 0x46, 2, CAM_CC,
			0x4E, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_3_dsp_clk", 0x46, 2, CAM_CC,
			0x4D, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_0_clk", 0x46, 2, CAM_CC,
			0x22, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_0_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x24, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_0_csid_clk", 0x46, 2, CAM_CC,
			0x23, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_1_clk", 0x46, 2, CAM_CC,
			0x38, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_1_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x3A, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_1_csid_clk", 0x46, 2, CAM_CC,
			0x39, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_2_clk", 0x46, 2, CAM_CC,
			0x55, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_2_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x57, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_2_csid_clk", 0x46, 2, CAM_CC,
			0x56, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_3_clk", 0x46, 2, CAM_CC,
			0x58, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_3_cphy_rx_clk", 0x46, 2, CAM_CC,
			0x5A, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ife_lite_3_csid_clk", 0x46, 2, CAM_CC,
			0x59, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_0_ahb_clk", 0x46, 2, CAM_CC,
			0x12, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_0_areg_clk", 0x46, 2, CAM_CC,
			0x11, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_0_axi_clk", 0x46, 2, CAM_CC,
			0x10, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_0_clk", 0x46, 2, CAM_CC,
			0xF, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_1_ahb_clk", 0x46, 2, CAM_CC,
			0x16, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_1_areg_clk", 0x46, 2, CAM_CC,
			0x15, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_1_axi_clk", 0x46, 2, CAM_CC,
			0x14, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_ipe_1_clk", 0x46, 2, CAM_CC,
			0x13, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_jpeg_clk", 0x46, 2, CAM_CC,
			0x25, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_lrme_clk", 0x46, 2, CAM_CC,
			0x2B, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk0_clk", 0x46, 2, CAM_CC,
			0x1, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk1_clk", 0x46, 2, CAM_CC,
			0x2, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk2_clk", 0x46, 2, CAM_CC,
			0x3, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk3_clk", 0x46, 2, CAM_CC,
			0x4, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk4_clk", 0x46, 2, CAM_CC,
			0x5D, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk5_clk", 0x46, 2, CAM_CC,
			0x5E, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk6_clk", 0x46, 2, CAM_CC,
			0x5F, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "cam_cc_mclk7_clk", 0x46, 2, CAM_CC,
			0x60, 0xFF, 0, 0xF, 0, 4, 0xD000, 0xD004, 0xD008 },
		{ "disp_cc_mdss_ahb_clk", 0x47, 2, DISP_CC,
			0x2B, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte0_clk", 0x47, 2, DISP_CC,
			0x15, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte0_intf_clk", 0x47, 2, DISP_CC,
			0x16, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte1_clk", 0x47, 2, DISP_CC,
			0x17, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_byte1_intf_clk", 0x47, 2, DISP_CC,
			0x18, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_crypto1_clk", 0x47, 2, DISP_CC,
			0x24, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_crypto_clk", 0x47, 2, DISP_CC,
			0x1D, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link1_clk", 0x47, 2, DISP_CC,
			0x22, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link1_intf_clk", 0x47, 2, DISP_CC,
			0x23, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link_clk", 0x47, 2, DISP_CC,
			0x1B, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_link_intf_clk", 0x47, 2, DISP_CC,
			0x1C, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_pixel1_clk", 0x47, 2, DISP_CC,
			0x1F, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_pixel2_clk", 0x47, 2, DISP_CC,
			0x21, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_dp_pixel_clk", 0x47, 2, DISP_CC,
			0x1E, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_edp_link_clk", 0x47, 2, DISP_CC,
			0x27, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_edp_link_intf_clk", 0x47, 2, DISP_CC,
			0x28, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_edp_pixel_clk", 0x47, 2, DISP_CC,
			0x26, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_mdp_clk", 0x47, 2, DISP_CC,
			0x11, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_mdp_lut_clk", 0x47, 2, DISP_CC,
			0x13, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_non_gdsc_ahb_clk", 0x47, 2, DISP_CC,
			0x2C, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_pclk0_clk", 0x47, 2, DISP_CC,
			0xF, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_pclk1_clk", 0x47, 2, DISP_CC,
			0x10, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_rot_clk", 0x47, 2, DISP_CC,
			0x12, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "disp_cc_mdss_rscc_ahb_clk", 0x47, 2, DISP_CC,
			0x2E, 0xFF, 0, 0x3, 0, 4, 0x7000, 0x5008, 0x500C },
		{ "gcc_aggre_ufs_card_2_axi_clk", 0x1CD, 2, GCC,
			0x1CD, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_aggre_ufs_card_axi_clk", 0x11E, 2, GCC,
			0x11E, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_aggre_ufs_phy_axi_clk", 0x11D, 2, GCC,
			0x11D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_aggre_usb3_mp_axi_clk", 0x1C8, 2, GCC,
			0x1C8, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_aggre_usb3_prim_axi_clk", 0x11B, 2, GCC,
			0x11B, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_aggre_usb3_sec_axi_clk", 0x11C, 2, GCC,
			0x11C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_cfg_noc_usb3_mp_axi_clk", 0x1C7, 2, GCC,
			0x1C7, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_cfg_noc_usb3_prim_axi_clk", 0x1D, 2, GCC,
			0x1D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_cfg_noc_usb3_sec_axi_clk", 0x1E, 2, GCC,
			0x1E, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_cpuss_ahb_clk", 0xCE, 2, GCC,
			0xCE, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_emac_ptp_clk", 0x1D3, 2, GCC,
			0x1D3, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_emac_rgmii_clk", 0x16A, 2, GCC,
			0x16A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gp1_clk", 0xDE, 2, GCC,
			0xDE, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gp2_clk", 0xDF, 2, GCC,
			0xDF, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gp3_clk", 0xE0, 2, GCC,
			0xE0, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gp4_clk", 0x17F, 2, GCC,
			0x17F, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gp5_clk", 0x180, 2, GCC,
			0x180, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gpu_gpll0_clk_src", 0x148, 2, GCC,
			0x148, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_gpu_gpll0_div_clk_src", 0x149, 2, GCC,
			0x149, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_npu_axi_clk", 0x1E9, 2, GCC,
			0x1E9, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_npu_gpll0_clk_src", 0x1A1, 2, GCC,
			0x1A1, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_npu_gpll0_div_clk_src", 0x1A2, 2, GCC,
			0x1A2, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie0_phy_refgen_clk", 0x1F3, 2, GCC,
			0x1F3, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie1_phy_refgen_clk", 0x164, 2, GCC,
			0x164, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie2_phy_refgen_clk", 0x163, 2, GCC,
			0x163, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie3_phy_refgen_clk", 0x162, 2, GCC,
			0x162, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_0_aux_clk", 0xE5, 2, GCC,
			0xE5, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_1_aux_clk", 0xEC, 2, GCC,
			0xEC, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_2_aux_clk", 0x175, 2, GCC,
			0x175, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_2_pipe_clk", 0x176, 2, GCC,
			0x176, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_2_slv_axi_clk", 0x172, 2, GCC,
			0x172, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_2_slv_q2a_axi_clk", 0x171, 2, GCC,
			0x171, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_aux_clk", 0x17C, 2, GCC,
			0x17C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_cfg_ahb_clk", 0x17B, 2, GCC,
			0x17B, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_mstr_axi_clk", 0x17A, 2, GCC,
			0x17A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_pipe_clk", 0x17D, 2, GCC,
			0x17D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_slv_axi_clk", 0x179, 2, GCC,
			0x179, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_3_slv_q2a_axi_clk", 0x178, 2, GCC,
			0x178, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pcie_phy_aux_clk", 0xEF, 2, GCC,
			0xEF, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pdm2_clk", 0x8E, 2, GCC,
			0x8E, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_pdm_ahb_clk", 0x8C, 2, GCC,
			0x8C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_prng_ahb_clk", 0x8F, 2, GCC,
			0x8F, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qspi_1_cnoc_periph_ahb_clk", 0x16F, 2, GCC,
			0x16F, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qspi_1_core_clk", 0x170, 2, GCC,
			0x170, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qspi_cnoc_periph_ahb_clk", 0x168, 2, GCC,
			0x168, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qspi_core_clk", 0x169, 2, GCC,
			0x169, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_core_2x_clk", 0x77, 2, GCC,
			0x77, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_core_clk", 0x76, 2, GCC,
			0x76, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s0_clk", 0x78, 2, GCC,
			0x78, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s1_clk", 0x79, 2, GCC,
			0x79, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s2_clk", 0x7A, 2, GCC,
			0x7A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s3_clk", 0x7B, 2, GCC,
			0x7B, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s4_clk", 0x7C, 2, GCC,
			0x7C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s5_clk", 0x7D, 2, GCC,
			0x7D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s6_clk", 0x7E, 2, GCC,
			0x7E, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap0_s7_clk", 0x1DA, 2, GCC,
			0x1DA, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_core_2x_clk", 0x83, 2, GCC,
			0x83, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_core_clk", 0x82, 2, GCC,
			0x82, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s0_clk", 0x84, 2, GCC,
			0x84, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s1_clk", 0x85, 2, GCC,
			0x85, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s2_clk", 0x86, 2, GCC,
			0x86, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s3_clk", 0x87, 2, GCC,
			0x87, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s4_clk", 0x88, 2, GCC,
			0x88, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap1_s5_clk", 0x89, 2, GCC,
			0x89, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_core_2x_clk", 0x198, 2, GCC,
			0x198, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_core_clk", 0x197, 2, GCC,
			0x197, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s0_clk", 0x199, 2, GCC,
			0x199, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s1_clk", 0x19A, 2, GCC,
			0x19A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s2_clk", 0x19B, 2, GCC,
			0x19B, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s3_clk", 0x19C, 2, GCC,
			0x19C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s4_clk", 0x19D, 2, GCC,
			0x19D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_qupv3_wrap2_s5_clk", 0x19E, 2, GCC,
			0x19E, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_sdcc2_ahb_clk", 0x71, 2, GCC,
			0x71, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_sdcc2_apps_clk", 0x70, 2, GCC,
			0x70, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_sdcc4_ahb_clk", 0x73, 2, GCC,
			0x73, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_sdcc4_apps_clk", 0x72, 2, GCC,
			0x72, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_sys_noc_cpuss_ahb_clk", 0xC, 2, GCC,
			0xC, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_tsif_ahb_clk", 0x90, 2, GCC,
			0x90, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_tsif_ref_clk", 0x91, 2, GCC,
			0x91, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_ahb_clk", 0x18B, 2, GCC,
			0x18B, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_axi_clk", 0x18A, 2, GCC,
			0x18A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_ice_core_clk", 0x191, 2, GCC,
			0x191, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_phy_aux_clk", 0x192, 2, GCC,
			0x192, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_rx_symbol_0_clk", 0x18D, 2, GCC,
			0x18D, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_rx_symbol_1_clk", 0x193, 2, GCC,
			0x193, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_tx_symbol_0_clk", 0x18C, 2, GCC,
			0x18C, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_2_unipro_core_clk", 0x190, 2, GCC,
			0x190, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_ahb_clk", 0xF1, 2, GCC,
			0xF1, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_axi_clk", 0xF0, 2, GCC,
			0xF0, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_ice_core_clk", 0xF7, 2, GCC,
			0xF7, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_rx_symbol_0_clk", 0xF3, 2, GCC,
			0xF3, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_rx_symbol_1_clk", 0xF9, 2, GCC,
			0xF9, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_tx_symbol_0_clk", 0xF2, 2, GCC,
			0xF2, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_card_unipro_core_clk", 0xF6, 2, GCC,
			0xF6, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_ahb_clk", 0xFC, 2, GCC,
			0xFC, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_axi_clk", 0xFB, 2, GCC,
			0xFB, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_ice_core_clk", 0x102, 2, GCC,
			0x102, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_rx_symbol_0_clk", 0xFE, 2, GCC,
			0xFE, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_rx_symbol_1_clk", 0x104, 2, GCC,
			0x104, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_tx_symbol_0_clk", 0xFD, 2, GCC,
			0xFD, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_ufs_phy_unipro_core_clk", 0x101, 2, GCC,
			0x101, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_mp_master_clk", 0x1AF, 2, GCC,
			0x1AF, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_mp_mock_utmi_clk", 0x1B1, 2, GCC,
			0x1B1, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_prim_master_clk", 0x5F, 2, GCC,
			0x5F, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_prim_mock_utmi_clk", 0x61, 2, GCC,
			0x61, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_sec_master_clk", 0x65, 2, GCC,
			0x65, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb30_sec_mock_utmi_clk", 0x67, 2, GCC,
			0x67, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_mp_phy_aux_clk", 0x1BD, 2, GCC,
			0x1BD, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_mp_phy_com_aux_clk", 0x1BE, 2, GCC,
			0x1BE, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_mp_phy_pipe_0_clk", 0x1BF, 2, GCC,
			0x1BF, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_mp_phy_pipe_1_clk", 0x1C0, 2, GCC,
			0x1C0, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_prim_phy_pipe_clk", 0x64, 2, GCC,
			0x64, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_usb3_sec_phy_pipe_clk", 0x6A, 2, GCC,
			0x6A, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_video_ahb_clk", 0x39, 2, GCC,
			0x39, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_video_axi0_clk", 0x3F, 2, GCC,
			0x3F, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_video_axi1_clk", 0x1A7, 2, GCC,
			0x1A7, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gcc_video_axic_clk", 0x1A8, 2, GCC,
			0x1A8, 0x3FF, 0, 0xF, 0, 2, 0x62000, 0x62004, 0x62008 },
		{ "gpu_cc_ahb_clk", 0x144, 2, GPU_CC,
			0x10, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_apb_clk", 0x144, 2, GPU_CC,
			0x14, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_gmu_clk", 0x144, 2, GPU_CC,
			0x18, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_qdss_at_clk", 0x144, 2, GPU_CC,
			0x12, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_qdss_trig_clk", 0x144, 2, GPU_CC,
			0x17, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_cx_snoc_dvm_clk", 0x144, 2, GPU_CC,
			0x15, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_gx_gmu_clk", 0x144, 2, GPU_CC,
			0xF, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "gpu_cc_gx_vsense_clk", 0x144, 2, GPU_CC,
			0xC, 0xFF, 0, 0x3, 0, 2, 0x1568, 0x10FC, 0x1100 },
		{ "npu_cc_armwic_core_clk", 0x1C6, 2, NPU_CC,
			0x4, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_cal_dp_cdc_clk", 0x1C6, 2, NPU_CC,
			0x8, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_cal_dp_clk", 0x1C6, 2, NPU_CC,
			0x1, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_comp_noc_axi_clk", 0x1C6, 2, NPU_CC,
			0x9, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_conf_noc_ahb_clk", 0x1C6, 2, NPU_CC,
			0xA, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_npu_core_atb_clk", 0x1C6, 2, NPU_CC,
			0xB, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_npu_core_clk", 0x1C6, 2, NPU_CC,
			0x2, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_npu_core_cti_clk", 0x1C6, 2, NPU_CC,
			0xC, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_npu_cpc_clk", 0x1C6, 2, NPU_CC,
			0x3, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "npu_cc_perf_cnt_clk", 0x1C6, 2, NPU_CC,
			0x10, 0xFF, 0, 0x3, 0, 2, 0x4000, 0x3004, 0x3008 },
		{ "video_cc_iris_ahb_clk", 0x48, 2, VIDEO_CC,
			0x7, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0x938, 0x940 },
		{ "video_cc_mvs0_core_clk", 0x48, 2, VIDEO_CC,
			0x3, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0x938, 0x940 },
		{ "video_cc_mvs1_core_clk", 0x48, 2, VIDEO_CC,
			0x5, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0x938, 0x940 },
		{ "video_cc_mvsc_core_clk", 0x48, 2, VIDEO_CC,
			0x1, 0x3F, 0, 0x7, 0, 5, 0xA4C, 0x938, 0x940 },
	),
	.hw.init = &(struct clk_init_data){
		.name = "gcc_debug_mux",
		.ops = &clk_debug_mux_ops,
		.parent_names = debug_mux_parent_names,
		.num_parents = ARRAY_SIZE(debug_mux_parent_names),
		.flags = CLK_IS_MEASURE,
	},
};

static const struct of_device_id clk_debug_match_table[] = {
	{ .compatible = "qcom,debugcc-sdmshrike" },
	{ }
};

static int map_debug_bases(struct platform_device *pdev, char *base, int cc)
{
	if (!of_get_property(pdev->dev.of_node, base, NULL))
		return -ENODEV;

	gcc_debug_mux.regmap[cc] =
			syscon_regmap_lookup_by_phandle(pdev->dev.of_node,
					base);
	if (IS_ERR(gcc_debug_mux.regmap[cc])) {
		pr_err("Failed to map %s (ret=%ld)\n", base,
				PTR_ERR(gcc_debug_mux.regmap[cc]));
		return PTR_ERR(gcc_debug_mux.regmap[cc]);
	}

	return 0;
}

static int clk_debug_sdmshrike_probe(struct platform_device *pdev)
{
	struct clk *clk;
	int ret;

	clk = devm_clk_get(&pdev->dev, "cxo");
	if (IS_ERR(clk)) {
		if (PTR_ERR(clk) != -EPROBE_DEFER)
			dev_err(&pdev->dev, "Unable to get xo clock\n");
		return PTR_ERR(clk);
	}

	debug_mux_priv.cxo = clk;

	gcc_debug_mux.regmap = devm_kcalloc(&pdev->dev, MAX_NUM_CC,
				sizeof(*gcc_debug_mux.regmap), GFP_KERNEL);
	if (!gcc_debug_mux.regmap)
		return -ENOMEM;

	ret = map_debug_bases(pdev, "qcom,gcc", GCC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,dispcc", DISP_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,videocc", VIDEO_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,camcc", CAM_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,npucc", NPU_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,gpucc", GPU_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,cpucc", CPU_CC);
	if (ret)
		return ret;

	ret = map_debug_bases(pdev, "qcom,mccc", MC_CC);
	if (ret)
		return ret;

	clk = devm_clk_register(&pdev->dev, &gcc_debug_mux.hw);
	if (IS_ERR(clk)) {
		dev_err(&pdev->dev, "Unable to register GCC debug mux\n");
		return PTR_ERR(clk);
	}

	ret = clk_debug_measure_register(&gcc_debug_mux.hw);
	if (ret)
		dev_err(&pdev->dev, "Could not register Measure clock\n");

	return ret;
}

static struct platform_driver clk_debug_driver = {
	.probe = clk_debug_sdmshrike_probe,
	.driver = {
		.name = "debugcc-sdmshrike",
		.of_match_table = clk_debug_match_table,
	},
};

static int __init clk_debug_sdmshrike_init(void)
{
	return platform_driver_register(&clk_debug_driver);
}
fs_initcall(clk_debug_sdmshrike_init);

static void __exit clk_debug_sdmshrike_exit(void)
{
	platform_driver_unregister(&clk_debug_driver);
}
module_exit(clk_debug_sdmshrike_exit);

MODULE_DESCRIPTION("QTI DEBUG CC sdmshrike Driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:debugcc-sdmshrike");