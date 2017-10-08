// COLMAP - Structure-from-Motion and Multi-View Stereo.
// Copyright (C) 2017  Johannes L. Schoenberger <jsch at inf.ethz.ch>
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "ui/dense_reconstruction_widget.h"

#include "base/undistortion.h"
#include "ui/main_window.h"

namespace colmap {
namespace {

const static std::string kFusedFileName = "fused.ply";
const static std::string kMeshedFileName = "meshed.ply";

class StereoOptionsTab : public OptionsWidget {
 public:
  StereoOptionsTab(QWidget* parent, OptionManager* options)
      : OptionsWidget(parent) {
    // Set a relatively small default image size to avoid too long computation.
    if (options->dense_stereo->max_image_size == -1) {
      options->dense_stereo->max_image_size = 2000;
    }

    AddOptionInt(&options->dense_stereo->max_image_size, "max_image_size", -1);
    AddOptionText(&options->dense_stereo->gpu_index, "gpu_index");
    AddOptionInt(&options->dense_stereo->window_radius, "window_radius");
    AddOptionDouble(&options->dense_stereo->sigma_spatial, "sigma_spatial");
    AddOptionDouble(&options->dense_stereo->sigma_color, "sigma_color");
    AddOptionInt(&options->dense_stereo->num_samples, "num_samples");
    AddOptionDouble(&options->dense_stereo->ncc_sigma, "ncc_sigma");
    AddOptionDouble(&options->dense_stereo->min_triangulation_angle,
                    "min_triangulation_angle");
    AddOptionDouble(&options->dense_stereo->incident_angle_sigma,
                    "incident_angle_sigma");
    AddOptionInt(&options->dense_stereo->num_iterations, "num_iterations");
    AddOptionBool(&options->dense_stereo->geom_consistency, "geom_consistency");
    AddOptionDouble(&options->dense_stereo->geom_consistency_regularizer,
                    "geom_consistency_regularizer");
    AddOptionDouble(&options->dense_stereo->geom_consistency_max_cost,
                    "geom_consistency_max_cost");
    AddOptionBool(&options->dense_stereo->filter, "filter");
    AddOptionDouble(&options->dense_stereo->filter_min_ncc, "filter_min_ncc");
    AddOptionDouble(&options->dense_stereo->filter_min_triangulation_angle,
                    "filter_min_triangulation_angle");
    AddOptionInt(&options->dense_stereo->filter_min_num_consistent,
                 "filter_min_num_consistent");
    AddOptionDouble(&options->dense_stereo->filter_geom_consistency_max_cost,
                    "filter_geom_consistency_max_cost");
    AddOptionDouble(&options->dense_stereo->cache_size,
                    "cache_size [gigabytes]", 0,
                    std::numeric_limits<double>::max(), 0.1, 1);
    AddOptionBool(&options->dense_stereo->write_consistency_graph,
                  "write_consistency_graph");
  }
};

class FusionOptionsTab : public OptionsWidget {
 public:
  FusionOptionsTab(QWidget* parent, OptionManager* options)
      : OptionsWidget(parent) {
    AddOptionInt(&options->dense_fusion->max_image_size, "max_image_size", -1);
    AddOptionInt(&options->dense_fusion->min_num_pixels, "min_num_pixels", 0);
    AddOptionInt(&options->dense_fusion->max_num_pixels, "max_num_pixels", 0);
    AddOptionInt(&options->dense_fusion->max_traversal_depth,
                 "max_traversal_depth", 1);
    AddOptionDouble(&options->dense_fusion->max_reproj_error,
                    "max_reproj_error", 0);
    AddOptionDouble(&options->dense_fusion->max_depth_error, "max_depth_error",
                    0, 1, 0.0001, 4);
    AddOptionDouble(&options->dense_fusion->max_normal_error,
                    "max_normal_error", 0, 180);
    AddOptionInt(&options->dense_fusion->check_num_images, "check_num_images",
                 1);
    AddOptionDouble(&options->dense_fusion->cache_size,
                    "cache_size [gigabytes]", 0,
                    std::numeric_limits<double>::max(), 0.1, 1);
  }
};

class MeshingOptionsTab : public OptionsWidget {
 public:
  MeshingOptionsTab(QWidget* parent, OptionManager* options)
      : OptionsWidget(parent) {
    AddOptionDouble(&options->dense_meshing->point_weight, "point_weight", 0);
    AddOptionInt(&options->dense_meshing->depth, "depth", 1);
    AddOptionDouble(&options->dense_meshing->color, "color", 0);
    AddOptionDouble(&options->dense_meshing->trim, "trim", 0);
    AddOptionInt(&options->dense_meshing->num_threads, "num_threads", -1);
  }
};

// Read the specified reference image names from a patch match configuration.
std::vector<std::pair<std::string, std::string>> ReadPatchMatchConfig(
    const std::string& config_path) {
  std::ifstream file(config_path);
  CHECK(file.is_open()) << config_path;

  std::string line;
  std::string ref_image_name;
  std::vector<std::pair<std::string, std::string>> images;
  while (std::getline(file, line)) {
    StringTrim(&line);

    if (line.empty() || line[0] == '#') {
      continue;
    }

    if (ref_image_name.empty()) {
      ref_image_name = line;
    } else {
      images.emplace_back(ref_image_name, line);
      ref_image_name.clear();
    }
  }

  return images;
}

}  // namespace

DenseReconstructionOptionsWidget::DenseReconstructionOptionsWidget(
    QWidget* parent, OptionManager* options)
    : QWidget(parent) {
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::ApplicationModal);
  setWindowTitle("Dense reconstruction options");

  QGridLayout* grid = new QGridLayout(this);

  QTabWidget* tab_widget = new QTabWidget(this);
  tab_widget->setElideMode(Qt::TextElideMode::ElideRight);
  tab_widget->addTab(new StereoOptionsTab(this, options), "Stereo");
  tab_widget->addTab(new FusionOptionsTab(this, options), "Fusion");
  tab_widget->addTab(new MeshingOptionsTab(this, options), "Meshing");

  grid->addWidget(tab_widget, 0, 0);
}

DenseReconstructionWidget::DenseReconstructionWidget(MainWindow* main_window,
                                                     OptionManager* options)
    : QWidget(main_window),
      main_window_(main_window),
      options_(options),
      reconstruction_(nullptr),
      thread_control_widget_(new ThreadControlWidget(this)),
      options_widget_(new DenseReconstructionOptionsWidget(this, options)),
      photometric_done_(false),
      geometric_done_(false) {
  setWindowFlags(Qt::Dialog);
  setWindowModality(Qt::ApplicationModal);
  setWindowTitle("Dense reconstruction");
  resize(main_window_->size().width() - 200,
         main_window_->size().height() - 20);

  QGridLayout* grid = new QGridLayout(this);

  undistortion_button_ = new QPushButton(tr("Undistortion"), this);
  connect(undistortion_button_, &QPushButton::released, this,
          &DenseReconstructionWidget::Undistort);
  grid->addWidget(undistortion_button_, 0, 0, Qt::AlignLeft);

  stereo_button_ = new QPushButton(tr("Stereo"), this);
  connect(stereo_button_, &QPushButton::released, this,
          &DenseReconstructionWidget::Stereo);
  grid->addWidget(stereo_button_, 0, 1, Qt::AlignLeft);

  fusion_button_ = new QPushButton(tr("Fusion"), this);
  connect(fusion_button_, &QPushButton::released, this,
          &DenseReconstructionWidget::Fusion);
  grid->addWidget(fusion_button_, 0, 2, Qt::AlignLeft);

  meshing_button_ = new QPushButton(tr("Meshing"), this);
  connect(meshing_button_, &QPushButton::released, this,
          &DenseReconstructionWidget::Meshing);
  grid->addWidget(meshing_button_, 0, 3, Qt::AlignLeft);

  QPushButton* options_button = new QPushButton(tr("Options"), this);
  connect(options_button, &QPushButton::released, options_widget_,
          &OptionsWidget::show);
  grid->addWidget(options_button, 0, 4, Qt::AlignLeft);

  QLabel* workspace_path_label = new QLabel("Workspace", this);
  grid->addWidget(workspace_path_label, 0, 5, Qt::AlignRight);

  workspace_path_text_ = new QLineEdit(this);
  grid->addWidget(workspace_path_text_, 0, 6, Qt::AlignRight);
  connect(workspace_path_text_, &QLineEdit::textChanged, this,
          &DenseReconstructionWidget::RefreshWorkspace, Qt::QueuedConnection);

  QPushButton* workspace_path_button = new QPushButton(tr("Select"), this);
  connect(workspace_path_button, &QPushButton::released, this,
          &DenseReconstructionWidget::SelectWorkspacePath,
          Qt::QueuedConnection);
  grid->addWidget(workspace_path_button, 0, 7, Qt::AlignRight);

  QStringList table_header;
  table_header << "image_name"
               << ""
               << "photometric"
               << "geometric"
               << "src_images";

  table_widget_ = new QTableWidget(this);
  table_widget_->setColumnCount(table_header.size());
  table_widget_->setHorizontalHeaderLabels(table_header);

  table_widget_->setShowGrid(true);
  table_widget_->setSelectionBehavior(QAbstractItemView::SelectRows);
  table_widget_->setSelectionMode(QAbstractItemView::SingleSelection);
  table_widget_->setEditTriggers(QAbstractItemView::NoEditTriggers);
  table_widget_->verticalHeader()->setDefaultSectionSize(25);

  grid->addWidget(table_widget_, 1, 0, 1, 8);

  grid->setColumnStretch(4, 1);

  image_viewer_widget_ = new ImageViewerWidget(this);
  image_viewer_widget_->setWindowFlags(Qt::Dialog);
  image_viewer_widget_->setWindowModality(Qt::ApplicationModal);

  refresh_workspace_action_ = new QAction(this);
  connect(refresh_workspace_action_, &QAction::triggered, this,
          &DenseReconstructionWidget::RefreshWorkspace);

  write_fused_points_action_ = new QAction(this);
  connect(write_fused_points_action_, &QAction::triggered, this,
          &DenseReconstructionWidget::WriteFusedPoints);

  show_meshing_info_action_ = new QAction(this);
  connect(show_meshing_info_action_, &QAction::triggered, this,
          &DenseReconstructionWidget::ShowMeshingInfo);

  RefreshWorkspace();
}

void DenseReconstructionWidget::showEvent(QShowEvent* event) {
  RefreshWorkspace();
}

void DenseReconstructionWidget::Show(Reconstruction* reconstruction) {
  reconstruction_ = reconstruction;
  show();
  raise();
}

void DenseReconstructionWidget::Undistort() {
  const std::string workspace_path = GetWorkspacePath();
  if (workspace_path.empty()) {
    return;
  }

  if (reconstruction_ == nullptr || reconstruction_->NumRegImages() < 2) {
    QMessageBox::critical(this, "",
                          tr("No reconstruction selected in main window"));
  }

  COLMAPUndistorter* undistorter =
      new COLMAPUndistorter(UndistortCameraOptions(), *reconstruction_,
                            *options_->image_path, workspace_path);
  undistorter->AddCallback(Thread::FINISHED_CALLBACK,
                           [this]() { refresh_workspace_action_->trigger(); });
  thread_control_widget_->StartThread("Undistorting...", true, undistorter);
}

void DenseReconstructionWidget::Stereo() {
  const std::string workspace_path = GetWorkspacePath();
  if (workspace_path.empty()) {
    return;
  }

#ifdef CUDA_ENABLED
  mvs::PatchMatchController* processor = new mvs::PatchMatchController(
      *options_->dense_stereo, workspace_path, "COLMAP", "");
  processor->AddCallback(Thread::FINISHED_CALLBACK,
                         [this]() { refresh_workspace_action_->trigger(); });
  thread_control_widget_->StartThread("Stereo...", true, processor);
#else
  QMessageBox::critical(this, "", tr("CUDA not supported"));
#endif
}

void DenseReconstructionWidget::Fusion() {
  const std::string workspace_path = GetWorkspacePath();
  if (workspace_path.empty()) {
    return;
  }

  std::string input_type;
  if (geometric_done_) {
    input_type = "geometric";
  } else if (photometric_done_) {
    input_type = "photometric";
  } else {
    QMessageBox::critical(this, "",
                          tr("All images must be processed prior to fusion"));
  }

  mvs::StereoFusion* fuser = new mvs::StereoFusion(
      *options_->dense_fusion, workspace_path, "COLMAP", input_type);
  fuser->AddCallback(Thread::FINISHED_CALLBACK, [this, fuser]() {
    fused_points_ = fuser->GetFusedPoints();
    write_fused_points_action_->trigger();
  });
  thread_control_widget_->StartThread("Fusion...", true, fuser);
}

void DenseReconstructionWidget::Meshing() {
  const std::string workspace_path = GetWorkspacePath();
  if (workspace_path.empty()) {
    return;
  }

  if (ExistsFile(JoinPaths(workspace_path, kFusedFileName))) {
    thread_control_widget_->StartFunction("Meshing...", [this,
                                                         workspace_path]() {
      mvs::PoissonReconstruction(*options_->dense_meshing,
                                 JoinPaths(workspace_path, kFusedFileName),
                                 JoinPaths(workspace_path, kMeshedFileName));
      show_meshing_info_action_->trigger();
    });
  }
}

void DenseReconstructionWidget::SelectWorkspacePath() {
  std::string workspace_path;
  if (workspace_path_text_->text().isEmpty()) {
    workspace_path = GetParentDir(*options_->project_path);
  } else {
    workspace_path = workspace_path_text_->text().toUtf8().constData();
  }

  workspace_path_text_->setText(QFileDialog::getExistingDirectory(
      this, tr("Select workspace path..."),
      QString::fromStdString(workspace_path), QFileDialog::ShowDirsOnly));

  RefreshWorkspace();
}

std::string DenseReconstructionWidget::GetWorkspacePath() {
  const std::string workspace_path =
      workspace_path_text_->text().toUtf8().constData();
  if (ExistsDir(workspace_path)) {
    return workspace_path;
  } else {
    QMessageBox::critical(this, "", tr("Invalid workspace path"));
    return "";
  }
}

void DenseReconstructionWidget::RefreshWorkspace() {
  table_widget_->clearContents();
  table_widget_->setRowCount(0);

  const std::string workspace_path =
      workspace_path_text_->text().toUtf8().constData();
  if (ExistsDir(workspace_path)) {
    undistortion_button_->setEnabled(true);
  } else {
    undistortion_button_->setEnabled(false);
    stereo_button_->setEnabled(false);
    fusion_button_->setEnabled(false);
    meshing_button_->setEnabled(false);
    return;
  }

  images_path_ = JoinPaths(workspace_path, "images");
  depth_maps_path_ = JoinPaths(workspace_path, "stereo/depth_maps");
  normal_maps_path_ = JoinPaths(workspace_path, "stereo/normal_maps");
  const std::string config_path =
      JoinPaths(workspace_path, "stereo/patch-match.cfg");

  if (ExistsDir(images_path_) && ExistsDir(depth_maps_path_) &&
      ExistsDir(normal_maps_path_) &&
      ExistsDir(JoinPaths(workspace_path, "sparse")) &&
      ExistsDir(JoinPaths(workspace_path, "stereo/consistency_graphs")) &&
      ExistsFile(config_path)) {
    stereo_button_->setEnabled(true);
  } else {
    stereo_button_->setEnabled(false);
    fusion_button_->setEnabled(false);
    meshing_button_->setEnabled(false);
    return;
  }

  const auto images = ReadPatchMatchConfig(config_path);
  table_widget_->setRowCount(images.size());

  for (size_t i = 0; i < images.size(); ++i) {
    const std::string image_name = images[i].first;
    const std::string src_images = images[i].second;
    const std::string image_path = JoinPaths(images_path_, image_name);

    QTableWidgetItem* image_name_item =
        new QTableWidgetItem(QString::fromStdString(image_name));
    table_widget_->setItem(i, 0, image_name_item);

    QPushButton* image_button = new QPushButton("Image");
    connect(image_button, &QPushButton::released,
            [this, image_name, image_path]() {
              image_viewer_widget_->setWindowTitle(
                  QString("Image for %1").arg(image_name.c_str()));
              image_viewer_widget_->ReadAndShow(image_path, true);
            });
    table_widget_->setCellWidget(i, 1, image_button);

    table_widget_->setCellWidget(
        i, 2, GenerateTableButtonWidget(image_name, "photometric"));
    table_widget_->setCellWidget(
        i, 3, GenerateTableButtonWidget(image_name, "geometric"));

    QTableWidgetItem* src_images_item =
        new QTableWidgetItem(QString::fromStdString(src_images));
    table_widget_->setItem(i, 4, src_images_item);
  }

  table_widget_->resizeColumnsToContents();

  fusion_button_->setEnabled(photometric_done_ || geometric_done_);
  meshing_button_->setEnabled(
      ExistsFile(JoinPaths(workspace_path, kFusedFileName)));
}

void DenseReconstructionWidget::WriteFusedPoints() {
  const int reply = QMessageBox::question(
      this, "",
      tr("Do you want to visualize the point cloud? Otherwise, to visualize "
         "the reconstructed dense point cloud later, navigate to the "
         "<i>dense</i> sub-folder in your workspace with <i>File > Import "
         "model from...</i>."),
      QMessageBox::Yes | QMessageBox::No);
  if (reply == QMessageBox::Yes) {
    const size_t reconstruction_idx =
        main_window_->reconstruction_manager_.Add();
    auto& reconstruction =
        main_window_->reconstruction_manager_.Get(reconstruction_idx);

    for (const auto& point : fused_points_) {
      const Eigen::Vector3d xyz(point.x, point.y, point.z);
      const point3D_t point3D_id = reconstruction.AddPoint3D(xyz, Track());
      const Eigen::Vector3ub rgb(point.r, point.g, point.b);
      reconstruction.Point3D(point3D_id).SetColor(rgb);
    }

    options_->render->min_track_len = 0;
    main_window_->reconstruction_manager_widget_->Update();
    main_window_->reconstruction_manager_widget_->SelectReconstruction(
        reconstruction_idx);
    main_window_->RenderNow();
  }

  const std::string workspace_path =
      workspace_path_text_->text().toUtf8().constData();
  if (workspace_path.empty()) {
    fused_points_ = {};
    return;
  }

  thread_control_widget_->StartFunction(
      "Exporting...", [this, workspace_path]() {
        mvs::WritePlyBinary(JoinPaths(workspace_path, kFusedFileName),
                            fused_points_);
        fused_points_ = {};
        meshing_button_->setEnabled(true);
      });
}

void DenseReconstructionWidget::ShowMeshingInfo() {
  QMessageBox::information(
      this, "",
      tr("To visualize the meshed model, you must use an external viewer such "
         "as Meshlab. The model is located in the workspace folder."));
}

QWidget* DenseReconstructionWidget::GenerateTableButtonWidget(
    const std::string& image_name, const std::string& type) {
  CHECK(type == "photometric" || type == "geometric");
  const bool photometric = type == "photometric";

  if (photometric) {
    photometric_done_ = true;
  } else {
    geometric_done_ = true;
  }

  const std::string depth_map_path =
      JoinPaths(depth_maps_path_,
                StringPrintf("%s.%s.bin", image_name.c_str(), type.c_str()));
  const std::string normal_map_path =
      JoinPaths(normal_maps_path_,
                StringPrintf("%s.%s.bin", image_name.c_str(), type.c_str()));

  QWidget* button_widget = new QWidget();
  QGridLayout* button_layout = new QGridLayout(button_widget);
  button_layout->setContentsMargins(0, 0, 0, 0);

  QPushButton* depth_map_button = new QPushButton("Depth map", button_widget);
  if (ExistsFile(depth_map_path)) {
    connect(depth_map_button, &QPushButton::released,
            [this, image_name, depth_map_path]() {
              mvs::DepthMap depth_map;
              depth_map.Read(depth_map_path);
              image_viewer_widget_->setWindowTitle(
                  QString("Depth map for %1").arg(image_name.c_str()));
              image_viewer_widget_->ShowBitmap(depth_map.ToBitmap(2, 98), true);
            });
  } else {
    depth_map_button->setEnabled(false);
    if (photometric) {
      photometric_done_ = false;
    } else {
      geometric_done_ = false;
    }
  }
  button_layout->addWidget(depth_map_button, 0, 1, Qt::AlignLeft);

  QPushButton* normal_map_button = new QPushButton("Normal map", button_widget);
  if (ExistsFile(normal_map_path)) {
    connect(normal_map_button, &QPushButton::released,
            [this, image_name, normal_map_path]() {
              mvs::NormalMap normal_map;
              normal_map.Read(normal_map_path);
              image_viewer_widget_->setWindowTitle(
                  QString("Normal map for %1").arg(image_name.c_str()));
              image_viewer_widget_->ShowBitmap(normal_map.ToBitmap(), true);
            });
  } else {
    normal_map_button->setEnabled(false);
    if (photometric) {
      photometric_done_ = false;
    } else {
      geometric_done_ = false;
    }
  }
  button_layout->addWidget(normal_map_button, 0, 2, Qt::AlignLeft);

  return button_widget;
}

}  // namespace colmap
